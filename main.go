package main

import (
	"fmt"
	"os"
	"strconv"
	"time"
)

func prime_sieve(n int, ch chan int) {
	start := time.Now()
	primes := make([]bool, n+1)
	for i := range primes {
		primes[i] = true
	}
	for i := 2; i*i <= n; i++ {
		if primes[i] {
			for j := i; j*i <= n; j++ {
				primes[j*i] = false
			}
		}
	}
	count := 0
	for i := 2; i <= n; i++ {
		if primes[i] {
			count++
		}
	}
	fmt.Printf("Sieve took: %v\n", time.Since(start))
	ch <- count // Send the count through the channel
}

func isPrime(n int) bool {
	if n <= 1 {
		return false
	}
	if n <= 3 {
		return true
	}
	if n%2 == 0 || n%3 == 0 {
		return false
	}
	i := 5
	for i*i <= n {
		if n%i == 0 || n%(i+2) == 0 {
			return false
		}
		i += 6
	}
	return true
}

func prime_normal(n int, ch chan int) {
	start := time.Now()
	count := 0
	for i := 2; i <= n; i++ {
		if isPrime(i) {
			count++
		}
	}
	fmt.Printf("Normal took: %v\n", time.Since(start))
	ch <- count // Send the count through the channel
}

func main() {
	if len(os.Args) != 2 {
		fmt.Println("Usage: go run main.go <number>")
		return
	}

	input, err := strconv.Atoi(os.Args[1])
	if err != nil {
		fmt.Println("Error: Please provide a valid number.")
		return
	}

	ch := make(chan int, 2) // Buffer the channel to avoid deadlock

	// Launching goroutines
	go prime_normal(input, ch)
	go prime_sieve(input, ch)

	// Receiving results from channels
	normalCount := <-ch
	sieveCount := <-ch

	// Printing results
	fmt.Printf("Normal: %d\n", normalCount)
	fmt.Printf("Sieve: %d\n", sieveCount)
}
