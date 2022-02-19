package cli

import "fmt"

func Color(color, text string) string {
	return fmt.Sprintf("[%s]%s[-]", color, text)
}
