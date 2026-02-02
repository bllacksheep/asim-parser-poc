package main

import (
	"errors"
	"fmt"
	tree_sitter_xml "github.com/tree-sitter-grammars/tree-sitter-xml/bindings/go"
	tree_sitter "github.com/tree-sitter/go-tree-sitter"
	"io"
	"log"
	"os"

	/*
	#cgo CFLAGS: -Iapi/
	#include "xsd.h"
	*/
	"C"
)

func sourceFile(file string) ([]byte, error) {
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
	if len(os.Args) < 2 {
		log.Fatal("Pass file name as arg")
	}

	file := os.Args[1]

	parser := tree_sitter.NewParser()
	defer parser.Close()
	parser.SetLanguage(tree_sitter.NewLanguage(tree_sitter_xml.LanguageXML()))

	data, err := sourceFile(file)
	if err != nil {
		log.Fatal(err)
	}

	tree := parser.Parse(data, nil)
	defer tree.Close()

	root := tree.RootNode()
	fmt.Println(root.ToSexp())
}
