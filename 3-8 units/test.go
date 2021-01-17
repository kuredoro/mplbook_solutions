package main

import (
	"errors"
	"fmt"
	"io/ioutil"
	"os"
	"os/exec"
	"path/filepath"
	"strings"

	"github.com/alexeyco/simpletable"
	"github.com/i582/cfmt"
)

type ErrorString string

func (e ErrorString) Error() string {
    return string(e)
}

const ErrCleanUp ErrorString = "clean up"

var (
    FailStr = cfmt.Sprint("{{FAIL}}::red|bold")
    PassStr = cfmt.Sprint("{{PASS}}::green|bold")
)

var PossibleCompilers = []string{"g++", "clang++"}

var OkDirs = []string{"./compiles"}
var FailDirs = []string{"./fails"}

func RunCommand(name string, args ...string) (string, error) {
    out, err := exec.Command(name, args...).CombinedOutput()
    return string(out), err
}

type TestDescription struct {
    Name string
    Filename string
    ShouldFail bool
}

func CollectTests(dirs []string, shouldFail bool) (tests []TestDescription) {
    for _, dir := range dirs {
        testFiles, err := filepath.Glob(filepath.Join(dir, "*.cpp"))
        if err != nil {
            fmt.Printf("error: collect tests: %v\n", err)
            continue
        }

        for _, testFile := range testFiles {
            name := testFile[:len(testFile) - len(".cpp")]
            tests = append(tests, TestDescription{name, testFile, shouldFail})
        }

        cfmt.Printf("{{===>}}::goodStep {{%d in}}::bold {{%s}}::cyan\n", 
            len(tests), dir)
    }

    return
}

func CompileAndRun(compiler, filename string) (bool, error) {
    tmpFile, err := ioutil.TempFile(os.TempDir(), "")
    if err != nil {
        return false, fmt.Errorf("test: %v", err)
    }
    tmpFile.Close()

    var exitError *exec.ExitError

    _, err = exec.Command(compiler, filename, "-I.", "-o", tmpFile.Name()).CombinedOutput()
    if errors.As(err, &exitError) {
//        fmt.Printf("compiler output:\n%s\n\n", string(out))
        return false, nil
    } else if err != nil {
        return false, fmt.Errorf("compile: %v", err)
    }

    _, err = exec.Command(tmpFile.Name()).Output()
    if errors.As(err, &exitError) {
        return false, nil
    } else if err != nil {
        return false, fmt.Errorf("run: %v", err)
    }

    err = os.Remove(tmpFile.Name())
    if err != nil {
        return true, fmt.Errorf("%w: %v", ErrCleanUp, err)
    }

    return true, nil
}

func main() {
    cfmt.RegisterStyle("status", func(s string) string {
        return cfmt.Sprintf("{{%s}}::blue|bold", s)
    })

    cfmt.RegisterStyle("goodStep", func(s string) string {
        return cfmt.Sprintf("{{%s}}::green|bold", s)
    })

    cfmt.Println("{{::}}::status {{Probing compilers}}::bold")
    var compilers []string
    for _, compiler := range PossibleCompilers {
        out, err := RunCommand(compiler, "--version")
        if err == nil {
            compilers = append(compilers, compiler)

            ver := strings.Split(out, "\n")[0]
            cfmt.Printf("{{===>}}::goodStep {{found}}::bold {{%s}}::magenta\n", ver)
        }
    }

    cfmt.Println("{{::}}::status {{Collecting tests}}::bold")

    var tests []TestDescription

    tests = append(tests, CollectTests(OkDirs, false)...)
    tests = append(tests, CollectTests(FailDirs, true)...)

    cfmt.Printf("{{--->}}::goodStep {{%d total}}::bold\n", len(tests))

    cfmt.Println("{{::}}::status {{Running tests}}::bold")

    var nameEraser string
    for _, test := range tests {
        if len(nameEraser) < len(test.Name) {
            nameEraser = strings.Repeat(" ", len(test.Name))
        }
    }

    statMat := make([][]bool, len(tests))
    for i := range statMat {
        statMat[i] = make([]bool, len(compilers))
    }

    testComplete := make(chan [2]int)
    for testID := range tests {
        for compilerID := range compilers {
            go func(testID, compilerID int) {
                test := tests[testID]
                compiler := compilers[compilerID]
                var err error
                statMat[testID][compilerID], err = CompileAndRun(compiler, test.Filename)
                
                if test.ShouldFail {
                    statMat[testID][compilerID] = !statMat[testID][compilerID]
                }

                if errors.Is(err, ErrCleanUp) {
                    fmt.Printf("warning: %v\n", err)
                } else if err != nil {
                    fmt.Printf("error: %v\n", err)
                }

                testComplete <- [2]int{testID, compilerID}
            }(testID, compilerID)
        }
    }

    jobCount := len(tests) * len(compilers)
    for i := 0; i < jobCount; i++ {
        pos := <-testComplete
        cfmt.Printf("\r[%d/%d] {{Compiled}}::bold %s", i+1, jobCount, nameEraser)
        cfmt.Printf("\r[%d/%d] {{Compiled}}::bold {{%s}}::yellow", 
            i+1, jobCount, tests[pos[0]].Name)
    }

    cfmt.Print("\n\n{{::}}::status {{Results}}::bold\n\n")

    table := simpletable.New()

    table.Header = &simpletable.Header{
        Cells: make([]*simpletable.Cell, 1 + len(compilers)),
    }

    table.Header.Cells[0] = &simpletable.Cell{
        Align: simpletable.AlignCenter, 
        Text: "Test name",
    }

    for i, compiler := range compilers {
        table.Header.Cells[i+1] = &simpletable.Cell{
            Align: simpletable.AlignCenter,
            Text: compiler,
        }
    }

    compilerResults := make([]bool, len(compilers))
    for i := range compilerResults {
        compilerResults[i] = true
    }

    for ti, test := range tests {
        r := make([]*simpletable.Cell, 1 + len(compilers))
        r[0] = &simpletable.Cell{
            Align: simpletable.AlignLeft,
            Text: test.Name,
        }

        for ci := range compilers {
            verdict := FailStr
            if statMat[ti][ci] {
                verdict = PassStr
            }

            compilerResults[ci] = compilerResults[ci] && statMat[ti][ci]

            r[ci+1] = &simpletable.Cell{
                Align: simpletable.AlignCenter,
                Text: verdict,
            }
        }

        table.Body.Cells = append(table.Body.Cells, r)
    }

    table.Footer = &simpletable.Footer{
        Cells: make([]*simpletable.Cell, 1 + len(compilers)),
    }

    table.Footer.Cells[0] = &simpletable.Cell{
        Align: simpletable.AlignLeft,
        Text: "Verdict",
    }

    for i, result := range compilerResults {
        verdict := FailStr
        if result {
            verdict = PassStr
        }

        table.Footer.Cells[i+1] = &simpletable.Cell{
            Align: simpletable.AlignCenter,
            Text: verdict,
        }
    }

    table.SetStyle(simpletable.StyleCompactLite)
    fmt.Println(table.String())
    fmt.Println()
}
