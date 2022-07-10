package main

import (
	"bufio"
	"container/list"
	"log"
	"os"
	"strings"

	graphviz "github.com/goccy/go-graphviz"
	"github.com/goccy/go-graphviz/cgraph"
)

type GrammarItem struct {
	name     string
	children []string
}
type StackItem struct {
	token string
	node  *cgraph.Node
}

func main() {

	g := graphviz.New()
	graph, err := g.Graph()
	if err != nil {
		log.Fatal(err)
	}
	defer func() {
		if err := graph.Close(); err != nil {
			log.Fatal(err)
		}
		g.Close()
	}()

	srcFile, dstFile := os.Args[1], os.Args[2]

	f, _ := os.Open(srcFile)
	scanner := bufio.NewScanner(f)
	grammar := list.New()
	for scanner.Scan() {
		line := strings.TrimSpace(scanner.Text())
		nodes := strings.Split(line, " -> ")
		grammar.PushFront(&GrammarItem{nodes[0], strings.Split(nodes[1], " ")})
	}
	n0, _ := graph.CreateNode(grammar.Front().Value.(*GrammarItem).name)
	stack := list.New()
	stack.PushBack(&StackItem{token: "ROOT", node: n0})
	for front := grammar.Front(); front != nil; front = grammar.Front() {
		grammarListBottom := front.Value.(*GrammarItem)
		for grammarListBottom.name != stack.Back().Value.(*StackItem).token && stack.Back() != stack.Front() {
			stack.Remove(stack.Back())
		}
		elem := stack.Back()
		for _, item := range grammarListBottom.children {
			n, _ := graph.CreateNode(item)
			stack.PushBack(&StackItem{token: item, node: n})
			graph.CreateEdge("", elem.Value.(*StackItem).node, n)
		}
		grammar.Remove(grammar.Front())
	}
	// 3. write to file directly
	if err := g.RenderFilename(graph, graphviz.PNG, dstFile); err != nil {
		log.Fatal(err)
	}
}
