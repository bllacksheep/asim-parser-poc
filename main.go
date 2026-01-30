package main

import (
    "fmt"
    tree_sitter "github.com/tree-sitter/go-tree-sitter"
    tree_sitter_xml "github.com/tree-sitter-grammars/tree-sitter-xml/bindings/go"
)

func main() {
    code := []byte("<root><child>hello</child></root>")

    parser := tree_sitter.NewParser()
	defer parser.Close()
    parser.SetLanguage(tree_sitter.NewLanguage(tree_sitter_xml.LanguageXML()))

    tree := parser.Parse(code, nil)
	defer tree.Close()

	root := tree.RootNode()
	fmt.Println(root.ToSexp())
}
