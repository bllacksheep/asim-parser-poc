package main

import (
	"errors"
	"fmt"
	tree_sitter_xml "github.com/tree-sitter-grammars/tree-sitter-xml/bindings/go"
	tree_sitter "github.com/tree-sitter/go-tree-sitter"
	"io"
	"log"
	"os"
)

func readFile(file string) ([]byte, error) {
	var data []byte
	buf := make([]byte, 512)
	f, err := os.Open(file)
	if err != nil {
		return nil, errors.New("Error reading from file")
	}
	for {
		n, err := f.Read(buf)
		if n > 0 {
			data = append(data, buf[:n]...)
		}

		if err != nil {
			if err == io.EOF {
				break
			}
			log.Fatal(err)
		}
	}
	return data, nil
}

func main() {
	parser := tree_sitter.NewParser()
	defer parser.Close()
	parser.SetLanguage(tree_sitter.NewLanguage(tree_sitter_xml.LanguageXML()))

	data, err := readFile("example.xml")

	if err != nil {
		panic(err)
	}

	tree := parser.Parse(data, nil)
	defer tree.Close()

	root := tree.RootNode()
	fmt.Println(root.ToSexp())
}
