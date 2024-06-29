package main
import (
  "fmt"
  "runtime"
)

type Person struct {
  name string
  age  int
  x    int
  y    int
}

var persons [0x10]*Person;

func create_persons() {
  for i:=0; i<0x10; i++ {
    var p *Person = &Person{fmt.Sprintf("Taro%d", i), 20, 0xdead, 0xbeef};
    if i % 2 == 0 {
      persons[i] = p;
    }
  }
}

func main() {
  create_persons();
  runtime.GC();
  for {} // infinity loop
}
