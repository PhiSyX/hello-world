package main

import (
	"bytes"
	"io"
	"os"
	"strings"
)

type myReaderStruct struct {
	src io.Reader
}

func (r *myReaderStruct) Read(buf []byte) (int, error) {
	tmp := make([]byte, len(buf))
	n, err := r.src.Read(tmp)
	copy(buf[:n], bytes.Title(tmp[:n]))
	return n, err
}

func NewReader(r io.Reader) io.Reader {
	return &myReaderStruct{src: r}
}

func main() {
	str_reader := strings.NewReader("Mon super reader")
	my_reader := NewReader(str_reader)
	io.Copy(os.Stdout, my_reader)
}
