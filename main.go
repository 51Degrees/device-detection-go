package main
import "C"
import (
	"dd"
	"fmt"
	"log"
	"os"
)

func main() {
	fs := "z:/goland/projects/src/device-detection-go-test/src/dd/device-detection-cxx/device-detection-data/51Degrees-LiteV3.2.dat"
	fh, error := os.Open(fs)
	if error != nil {

	}
	log.Println(fh.Name())
	manager := dd.Create(fs,"")
	manager.Process("Nokia3510i/1.0")
	result := manager.Get("IsMobile")
	fmt.Println(result)
}
