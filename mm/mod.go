package mm

import "runtime"

// --------- //
// Structure //
// --------- //

type Memory struct {
	Alloc float64
}

// -------------- //
// Implémentation //
// -------------- //

func NewMemory() *Memory {
	var mm runtime.MemStats
	runtime.ReadMemStats(&mm)
	return &Memory{
		Alloc: float64(mm.HeapAlloc) / 1024 / 1024,
	}
}
