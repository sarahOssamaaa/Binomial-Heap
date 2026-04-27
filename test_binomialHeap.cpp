#include <iostream>
#include <chrono>
#include <iomanip>
#include <vector>
#include <cmath>
#include "binomialheap.hpp"
#include <cassert>
using namespace std;

void testBasicOperations() {
    cout << "=== Testing Basic Operations ===" << endl;

    BinomialHeap<int> heap;

    // Test 1: Insert and getMin
    cout << "\nTest 1: Insert and getMin" << endl;
    heap.insert(5);
    cout << "After inserting 5, getMin: " << heap.getMin() << " (Expected: 5)" << endl;

    heap.insert(3);
    cout << "After inserting 3, getMin: " << heap.getMin() << " (Expected: 3)" << endl;

    heap.insert(8);
    cout << "After inserting 8, getMin: " << heap.getMin() << " (Expected: 3)" << endl;

    heap.insert(1);
    cout << "After inserting 1, getMin: " << heap.getMin() << " (Expected: 1)" << endl;

    // Test 2: ExtractMin sequence
    cout << "\nTest 2: ExtractMin sequence" << endl;
    cout << "Expected extraction order: 1, 3, 5, 8" << endl;
    cout << "Actual extraction:" << endl;

    try {
        cout << "1. extractMin(): " << heap.extractMin() << " (Expected: 1)" << endl;
        cout << "2. extractMin(): " << heap.extractMin() << " (Expected: 3)" << endl;
        cout << "3. extractMin(): " << heap.extractMin() << " (Expected: 5)" << endl;
        cout << "4. extractMin(): " << heap.extractMin() << " (Expected: 8)" << endl;
    }
    catch (const runtime_error& e) {
        cout << "Unexpected error: " << e.what() << endl;
    }

    cout << "✓ Test 2 passed if values extracted in order 1,3,5,8" << endl;
}
void testEdgeCases() {
    cout << "\n=== Testing Edge Cases ===" << endl;

    // Test 1: Empty heap operations
    cout << "\nTest 1: Empty heap operations" << endl;
    BinomialHeap<int> emptyHeap;

    try {
        emptyHeap.getMin();
        cout << "✗ ERROR: Should have thrown exception!" << endl;
    }
    catch (const runtime_error& e) {
        cout << "✓ getMin() " << e.what() << endl;
        cout << "  Expected: 'Cannot get min from empty heap'" << endl;
    }

    try {
        emptyHeap.extractMin();
        cout << "✗ ERROR: Should have thrown exception!" << endl;
    }
    catch (const runtime_error& e) {
        cout << "✓ extractMin() " << e.what() << endl;
        cout << "  Expected: 'Cannot extract min from empty heap'" << endl;
    }

    // Test 2: Single element
    cout << "\nTest 2: Single element heap" << endl;
    BinomialHeap<int> singleHeap;
    singleHeap.insert(42);
    cout << "Single element heap getMin(): " << singleHeap.getMin()
        << " (Expected: 42)" << endl;
    cout << "Single element extractMin(): " << singleHeap.extractMin()
        << " (Expected: 42)" << endl;

    // Verify heap is now empty
    try {
        singleHeap.getMin();
        cout << "✗ ERROR: Should be empty after extraction!" << endl;
    }
    catch (const runtime_error& e) {
        cout << "✓ Heap correctly empty after single extraction" << endl;
    }

    // Test 3: Duplicate values
    cout << "\nTest 3: Duplicate values" << endl;
    BinomialHeap<int> dupHeap;
    dupHeap.insert(5);
    dupHeap.insert(5);
    dupHeap.insert(5);
    cout << "Duplicate values getMin(): " << dupHeap.getMin()
        << " (Expected: 5)" << endl;
    cout << "Extract first: " << dupHeap.extractMin() << " (Expected: 5)" << endl;
    cout << "Extract second: " << dupHeap.extractMin() << " (Expected: 5)" << endl;
    cout << "Extract third: " << dupHeap.extractMin() << " (Expected: 5)" << endl;

    // Test 4: Negative values
    cout << "\nTest 4: Negative values" << endl;
    BinomialHeap<int> negHeap;
    negHeap.insert(-5);
    negHeap.insert(10);
    negHeap.insert(-3);
    cout << "getMin() with negatives: " << negHeap.getMin()
        << " (Expected: -5)" << endl;
    cout << "Extract sequence: " << negHeap.extractMin() << " (Expected: -5), "
        << negHeap.extractMin() << " (Expected: -3), "
        << negHeap.extractMin() << " (Expected: 10)" << endl;
}
void testVisualOperations() {
    cout << "\n=== Visual Structure Tests ===" << endl;

    // Test 1: Insert in reverse order (forces tree merging)
    cout << "\nTest 1: Insert 16, 15, 14, ..., 1" << endl;
    BinomialHeap<int> heap;

    cout << "Inserting numbers 16 down to 1..." << endl;
    for (int i = 16; i >= 1; i--) {
        heap.insert(i);
    }

    cout << "Current getMin(): " << heap.getMin() << " (Expected: 1)" << endl;

    // Extract all and verify sorted order
    cout << "\nExtracting all elements (should be in order 1..16):" << endl;
    int prev = -1000;
    int count = 0;
    bool error = false;

    for (int expected = 1; expected <= 16; expected++) {
        try {
            int current = heap.extractMin();
            cout << "Extracted: " << current << " (Expected: " << expected << ")";

            if (current != expected) {
                cout << " ✗ MISMATCH!" << endl;
                error = true;
            }
            else {
                cout << " ✓" << endl;
            }

            // Verify heap property (should be increasing)
            if (current < prev) {
                cout << "✗ ERROR: Heap property violated! "
                    << prev << " > " << current << endl;
                error = true;
            }
            prev = current;
            count++;

        }
        catch (...) {
            cout << "✗ ERROR: Heap empty too soon!" << endl;
            error = true;
            break;
        }
    }

    if (!error) {
        cout << "✓ Perfect! All 16 elements extracted in correct order" << endl;
    }

    // Test 2: Powers of 2 (creates single B₄ tree)
    cout << "\nTest 2: Insert 1, 2, 4, 8, 16 (powers of 2)" << endl;
    BinomialHeap<int> heap2;
    heap2.insert(1);
    heap2.insert(2);
    heap2.insert(4);
    heap2.insert(8);
    heap2.insert(16);

    cout << "Extracting from heap with powers of 2:" << endl;
    int expected2[] = { 1, 2, 4, 8, 16 };
    for (int i = 0; i < 5; i++) {
        int val = heap2.extractMin();
        cout << "Extracted: " << val << " (Expected: " << expected2[i] << ")";
        if (val == expected2[i]) {
            cout << " ✓" << endl;
        }
        else {
            cout << " ✗" << endl;
        }
    }
}
void testDecreaseKey() {
    cout << "\n=== Testing DecreaseKey ===" << endl;

    BinomialHeap<int> heap;

    // Test 1: Basic decreaseKey
    cout << "\nTest 1: Basic decreaseKey operation" << endl;
    heap.insert(10);
    heap.insert(20);
    heap.insert(30);
    heap.insert(40);
    heap.insert(50);

    cout << "Initial getMin(): " << heap.getMin() << " (Expected: 10)" << endl;

    // Decrease 40 to 5 (should become new min)
    try {
        heap.decreaseKey(40, 5);
        cout << "After decreasing 40 to 5, getMin(): " << heap.getMin()
            << " (Expected: 5)" << endl;
    }
    catch (const runtime_error& e) {
        cout << "✗ Unexpected error: " << e.what() << endl;
    }

    // Extract to verify order
    cout << "\nExtracting to verify new order:" << endl;
    cout << "Expected order: 5 (was 40), 10, 20, 30, 50" << endl;
    cout << "Actual extraction:" << endl;
    try {
        cout << "1. " << heap.extractMin() << " (Expected: 5)" << endl;
        cout << "2. " << heap.extractMin() << " (Expected: 10)" << endl;
        cout << "3. " << heap.extractMin() << " (Expected: 20)" << endl;
        cout << "4. " << heap.extractMin() << " (Expected: 30)" << endl;
        cout << "5. " << heap.extractMin() << " (Expected: 50)" << endl;
    }
    catch (...) {
        cout << "✗ Heap empty too soon!" << endl;
    }

    // Test 2: Error cases
    cout << "\nTest 2: Error handling" << endl;
    BinomialHeap<int> heap2;
    heap2.insert(100);
    heap2.insert(200);

    // Try to decrease non-existent key
    try {
        heap2.decreaseKey(999, 1);
        cout << "✗ ERROR: Should have thrown for non-existent key!" << endl;
    }
    catch (const runtime_error& e) {
        cout << "✓ Correctly throws for non-existent key: " << e.what() << endl;
        cout << "  Expected: 'Key not found in heap'" << endl;
    }

    // Try to increase value (new value > old value)
    try {
        heap2.decreaseKey(200, 250);
        cout << "✗ ERROR: Should have rejected increase!" << endl;
    }
    catch (const runtime_error& e) {
        cout << "✓ Correctly rejects increase: " << e.what() << endl;
        cout << "  Expected: 'New value must be smaller than current value'" << endl;
    }

    // Test 3: Multiple decreaseKey operations
    cout << "\nTest 3: Multiple decreaseKey operations" << endl;
    BinomialHeap<int> heap3;
    heap3.insert(100);
    heap3.insert(90);
    heap3.insert(80);
    heap3.insert(70);

    cout << "Initial min: " << heap3.getMin() << " (Expected: 70)" << endl;
    heap3.decreaseKey(100, 65);
    cout << "After decreasing 100→65, min: " << heap3.getMin() << " (Expected: 65)" << endl;
    heap3.decreaseKey(80, 60);
    cout << "After decreasing 80→60, min: " << heap3.getMin() << " (Expected: 60)" << endl;
    heap3.decreaseKey(65, 55);
    cout << "After decreasing 65→55, min: " << heap3.getMin() << " (Expected: 55)" << endl;
}
void testInsertOperations() {
    cout << "========================================" << endl;
    cout << "     TESTING INSERT OPERATIONS          " << endl;
    cout << "========================================" << endl;

    // Test 1: Basic sequential insert
    cout << "\nTest 1: Basic sequential insert (checking min after each insert)" << endl;
    BinomialHeap<int> heap1;

    cout << "Insert 50 -> getMin: ";
    heap1.insert(50);
    cout << heap1.getMin() << " (Expected: 50)" << endl;

    cout << "Insert 30 -> getMin: ";
    heap1.insert(30);
    cout << heap1.getMin() << " (Expected: 30)" << endl;

    cout << "Insert 70 -> getMin: ";
    heap1.insert(70);
    cout << heap1.getMin() << " (Expected: 30)" << endl;

    cout << "Insert 20 -> getMin: ";
    heap1.insert(20);
    cout << heap1.getMin() << " (Expected: 20)" << endl;

    cout << "Insert 10 -> getMin: ";
    heap1.insert(10);
    cout << heap1.getMin() << " (Expected: 10)" << endl;

    // Verify by extracting all
    cout << "\nExtracting all to verify heap property:" << endl;
    int expected1[] = { 10, 20, 30, 50, 70 };
    for (int i = 0; i < 5; i++) {
        int val = heap1.extractMin();
        cout << "Extracted: " << val << " (Expected: " << expected1[i] << ")";
        if (val == expected1[i]) {
            cout << " ✓" << endl;
        }
        else {
            cout << " ✗" << endl;
        }
    }

    // Test 2: Insert in sorted order (ascending)
    cout << "\nTest 2: Insert in ascending order (1, 2, 3, 4, 5)" << endl;
    BinomialHeap<int> heap2;
    for (int i = 1; i <= 5; i++) {
        heap2.insert(i);
    }
    cout << "getMin: " << heap2.getMin() << " (Expected: 1)" << endl;

    // Test 3: Insert in sorted order (descending) 
    cout << "\nTest 3: Insert in descending order (5, 4, 3, 2, 1)" << endl;
    BinomialHeap<int> heap3;
    for (int i = 5; i >= 1; i--) {
        heap3.insert(i);
    }
    cout << "getMin: " << heap3.getMin() << " (Expected: 1)" << endl;

    // Test 4: Insert same value multiple times
    cout << "\nTest 4: Insert same value (7) 5 times" << endl;
    BinomialHeap<int> heap4;
    for (int i = 0; i < 5; i++) {
        heap4.insert(7);
    }
    cout << "getMin: " << heap4.getMin() << " (Expected: 7)" << endl;

    // Extract all 7's
    for (int i = 0; i < 5; i++) {
        int val = heap4.extractMin();
        cout << "Extract " << (i + 1) << ": " << val << " (Expected: 7)";
        if (val == 7) cout << " ✓" << endl;
        else cout << " ✗" << endl;
    }

    // Test 5: Insert to create binomial trees of different orders
    cout << "\nTest 5: Insert 8 elements to create B₃ tree" << endl;
    cout << "Inserting: 8, 7, 6, 5, 4, 3, 2, 1" << endl;
    BinomialHeap<int> heap5;
    for (int i = 8; i >= 1; i--) {
        heap5.insert(i);
    }
    cout << "getMin: " << heap5.getMin() << " (Expected: 1)" << endl;

    // Extract all (should be sorted)
    cout << "Extracting all (should be 1 through 8):" << endl;
    bool correct = true;
    for (int i = 1; i <= 8; i++) {
        int val = heap5.extractMin();
        cout << val << " ";
        if (val != i) correct = false;
    }
    cout << endl;
    cout << (correct ? "✓ All correct!" : "✗ Some values wrong!") << endl;

    // Test 6: Insert mixed positive and negative
    cout << "\nTest 6: Insert mixed positive and negative values" << endl;
    BinomialHeap<int> heap6;
    heap6.insert(100);
    heap6.insert(-50);
    heap6.insert(0);
    heap6.insert(-100);
    heap6.insert(50);

    cout << "Extraction order should be: -100, -50, 0, 50, 100" << endl;
    cout << "Actual extraction: ";
    int expected6[] = { -100, -50, 0, 50, 100 };
    bool correct6 = true;
    for (int i = 0; i < 5; i++) {
        int val = heap6.extractMin();
        cout << val << " ";
        if (val != expected6[i]) correct6 = false;
    }
    cout << endl;
    cout << (correct6 ? "✓ Correct order!" : "✗ Wrong order!") << endl;

    // Test 7: Insert large number of elements
    cout << "\nTest 7: Insert 100 random elements" << endl;
    BinomialHeap<int> heap7;
    srand(42); // Fixed seed for reproducibility

    for (int i = 0; i < 100; i++) {
        heap7.insert(rand() % 1000);
    }

    // Verify heap property by extracting all
    cout << "Extracting 100 elements (should be in sorted order):" << endl;
    int prev = -1;
    bool heapPropertyValid = true;
    int count = 0;

    while (true) {
        try {
            int current = heap7.extractMin();
            count++;

            if (current < prev) {
                heapPropertyValid = false;
                cout << "✗ Heap property violated: " << prev << " > " << current << endl;
            }
            prev = current;
        }
        catch (...) {
            break;
        }
    }

    cout << "Extracted " << count << " elements" << endl;
    cout << (heapPropertyValid ? "✓ Heap property maintained!" : "✗ Heap property violated!") << endl;

    // Test 8: Insert after extraction
    cout << "\nTest 8: Insert after partial extraction" << endl;
    BinomialHeap<int> heap8;
    heap8.insert(10);
    heap8.insert(20);
    heap8.insert(30);

    cout << "Initial min: " << heap8.getMin() << " (Expected: 10)" << endl;
    cout << "Extract min: " << heap8.extractMin() << " (Expected: 10)" << endl;

    cout << "Insert 5 after extraction -> getMin: ";
    heap8.insert(5);
    cout << heap8.getMin() << " (Expected: 5)" << endl;

    cout << "Insert 15 after extraction -> getMin: ";
    heap8.insert(15);
    cout << heap8.getMin() << " (Expected: 5)" << endl;

    // Extract remaining
    cout << "\nExtract remaining (should be: 5, 15, 20, 30):" << endl;
    int remainingExpected[] = { 5, 15, 20, 30 };
    for (int i = 0; i < 4; i++) {
        int val = heap8.extractMin();
        cout << "Extracted: " << val << " (Expected: " << remainingExpected[i] << ")";
        if (val == remainingExpected[i]) cout << " ✓" << endl;
        else cout << " ✗" << endl;
    }

    cout << "\n========================================" << endl;
    cout << "     INSERT TESTS COMPLETE              " << endl;
    cout << "========================================" << endl;
}

void testInsertTimeComplexity() {
    cout << "========================================" << endl;
    cout << "  TESTING INSERT TIME COMPLEXITY (O(1))" << endl;
    cout << "========================================" << endl;

    const int SIZES[] = { 1000, 10000, 100000, 500000 };
    const int NUM_TESTS = 5;

    for (int size : SIZES) {
        cout << "\nTesting with " << size << " elements:" << endl;

        double totalTime = 0;

        for (int test = 0; test < NUM_TESTS; test++) {
            BinomialHeap<int> heap;

            auto start = chrono::high_resolution_clock::now();

            // Insert elements
            for (int i = 0; i < size; i++) {
                heap.insert(rand() % 1000000);
            }

            auto end = chrono::high_resolution_clock::now();
            chrono::duration<double> duration = end - start;
            totalTime += duration.count();

            cout << "  Test " << (test + 1) << ": " << duration.count()
                << " seconds" << endl;
        }

        double avgTime = totalTime / NUM_TESTS;
        double timePerInsert = avgTime / size * 1e9; // nanoseconds per insert

        cout << "  Average: " << avgTime << " seconds" << endl;
        cout << "  Time per insert: " << timePerInsert << " nanoseconds" << endl;

        // For O(1), time per insert should remain constant as size increases
        cout << "  Complexity check: ";
        if (size >= 10000) {
            // Rough check: time per insert should not increase dramatically
            if (timePerInsert < 500) { // Adjust threshold based on your system
                cout << "✓ Consistent with O(1)" << endl;
            }
            else {
                cout << "✗ Might not be O(1) (too slow)" << endl;
            }
        }
    }
}
void testGetMinTimeComplexity() {
    cout << "\n========================================" << endl;
    cout << "  TESTING getMin TIME COMPLEXITY (O(log n))" << endl;
    cout << "========================================" << endl;

    const int SIZES[] = { 1000, 10000, 100000, 500000 };

    for (int size : SIZES) {
        cout << "\nTesting getMin with heap of size " << size << ":" << endl;

        // Build heap
        BinomialHeap<int> heap;
        for (int i = 0; i < size; i++) {
            heap.insert(rand() % 1000000);
        }

        // Time multiple getMin operations
        const int NUM_CALLS = 10000;
        auto start = chrono::high_resolution_clock::now();

        for (int i = 0; i < NUM_CALLS; i++) {
            volatile int min = heap.getMin(); // volatile to prevent optimization
        }

        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> duration = end - start;
        double timePerCall = duration.count() / NUM_CALLS * 1e9;

        cout << "  Time per getMin: " << timePerCall << " nanoseconds" << endl;

        // For O(log n), time should increase slowly with size
        if (size == 1000) {
            cout << "  (Baseline for comparison)" << endl;
        }
    }
}
void testExtractMinTimeComplexity() {
    cout << "\n========================================" << endl;
    cout << "  TESTING extractMin TIME COMPLEXITY (O(log n))" << endl;
    cout << "========================================" << endl;

    const int SIZES[] = { 1000, 10000, 50000, 100000 };

    for (int size : SIZES) {
        cout << "\nExtracting all " << size << " elements:" << endl;

        // Build heap
        BinomialHeap<int> heap;
        for (int i = 0; i < size; i++) {
            heap.insert(rand() % 1000000);
        }

        auto start = chrono::high_resolution_clock::now();

        // Extract all elements
        int extracted = 0;
        while (extracted < size) {
            heap.extractMin();
            extracted++;
        }

        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> duration = end - start;
        double totalTime = duration.count();
        double timePerExtract = totalTime / size * 1e9;

        cout << "  Total time: " << totalTime << " seconds" << endl;
        cout << "  Time per extractMin: " << timePerExtract << " nanoseconds" << endl;

        // For O(log n), time per extract should increase slowly
        if (size == 1000) {
            cout << "  (Baseline for comparison)" << endl;
        }
    }
}
void testDecreaseKeyTimeComplexity() {
    cout << "\n========================================" << endl;
    cout << "  TESTING decreaseKey TIME COMPLEXITY (O(log n))" << endl;
    cout << "========================================" << endl;

    const int SIZES[] = { 1000, 5000, 10000, 20000 };

    for (int size : SIZES) {
        cout << "\nTesting with heap of size " << size << ":" << endl;

        // Build heap with known values
        BinomialHeap<int> heap;
        vector<int> values;
        for (int i = 0; i < size; i++) {
            int val = 1000 + i; // Ensure unique values
            heap.insert(val);
            values.push_back(val);
        }

        // Time multiple decreaseKey operations
        const int NUM_OPERATIONS = min(1000, size);
        auto start = chrono::high_resolution_clock::now();

        for (int i = 0; i < NUM_OPERATIONS; i++) {
            int oldVal = values[i];
            int newVal = oldVal - 500; // Decrease by significant amount
            heap.decreaseKey(oldVal, newVal);
        }

        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> duration = end - start;
        double timePerOperation = duration.count() / NUM_OPERATIONS * 1e9;

        cout << "  Time per decreaseKey: " << timePerOperation << " nanoseconds" << endl;

        if (size == 1000) {
            cout << "  (Baseline for comparison)" << endl;
        }
    }
}
void testUnionTimeComplexity() {
    cout << "\n========================================" << endl;
    cout << "  TESTING union TIME COMPLEXITY" << endl;
    cout << "========================================" << endl;

    // Test 1: Union of two empty heaps
    cout << "\nTest 1: Union of empty heaps" << endl;
    {
        BinomialHeap<int> heap1, heap2;
        // We can't directly call unionHeap since it's protected
        // But we can test through insert/extract patterns
    }

    // Test 2: Union of heaps with single elements
    cout << "\nTest 2: Direct union test (small)" << endl;

    // We need to make unionHeap public temporarily or use friend
    // For now, let's test through insert patterns

    // Test 3: Measure union through insert sequence
    cout << "\nTest 3: Union complexity through insert pattern" << endl;

    vector<int> sizes = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024 };

    for (int size : sizes) {
        cout << "\nTesting with " << size << " elements inserted in descending order:" << endl;

        BinomialHeap<int> heap;
        auto totalStart = chrono::high_resolution_clock::now();

        // Insert in descending order (forces union operations)
        for (int i = size; i >= 1; i--) {
            heap.insert(i);
        }

        auto totalEnd = chrono::high_resolution_clock::now();
        double totalTime = chrono::duration<double>(totalEnd - totalStart).count();

        cout << "  Total insert time: " << totalTime << " seconds" << endl;
        cout << "  Time per insert: " << (totalTime / size * 1e9) << " ns" << endl;

        // Verify heap property
        bool valid = true;
        int prev = -1000000;
        for (int i = 0; i < size; i++) {
            int current = heap.extractMin();
            if (current < prev) {
                valid = false;
                cout << "  ERROR: Heap property violated at position " << i
                    << ": " << prev << " > " << current << endl;
                break;
            }
            prev = current;
        }

        if (valid) {
            cout << "  ✓ Heap property maintained" << endl;
        }
    }

    // Test 4: Union of two large heaps
    cout << "\nTest 4: Union of two pre-built heaps" << endl;

    // We need to make unionHeap accessible for this test
    // Let's create a helper class or modify visibility temporarily
}



void test_clear_empty_heap() {
    BinomialHeap<int> h;
    h.clear();
    assert(h.isEmpty());
    cout << "test_clear_empty_heap passed\n";
}

/* ---------------- TEST 2 ---------------- */
void test_clear_single_node() {
    BinomialHeap<int> h;
    h.insert(10);
    h.clear();
    assert(h.isEmpty());
    cout << "test_clear_single_node passed\n";
}

/* ---------------- TEST 3 ---------------- */
void test_clear_multiple_inserts() {
    BinomialHeap<int> h;
    h.insert(10);
    h.insert(3);
    h.insert(7);
    h.insert(1);
    h.insert(20);

    h.clear();
    assert(h.isEmpty());
    cout << "test_clear_multiple_inserts passed\n";
}

/* ---------------- TEST 4 ---------------- */
void test_clear_deep_heap() {
    BinomialHeap<int> h;

    // Create deeper trees
    for (int i = 100; i >= 0; --i) {
        h.insert(i);
    }

    h.clear();
    assert(h.isEmpty());
    cout << "test_clear_deep_heap passed\n";
}

/* ---------------- TEST 5 ---------------- */
void test_clear_twice() {
    BinomialHeap<int> h;
    h.insert(5);
    h.insert(2);

    h.clear();
    h.clear();  // should NOT crash
    assert(h.isEmpty());
    cout << "test_clear_twice passed\n";
}

/* ---------------- TEST 6 ---------------- */
void test_heap_usable_after_clear() {
    BinomialHeap<int> h;
    h.insert(10);
    h.insert(5);

    h.clear();

    h.insert(42);
    assert(!h.isEmpty());
    assert(h.getMin() == 42);

    cout << "test_heap_usable_after_clear passed\n";
}



void test_isEmpty_basic() {
    BinomialHeap<int> h;
    assert(h.isEmpty());      // empty heap

    h.insert(10);
    assert(!h.isEmpty());     // not empty

    h.clear();
    assert(h.isEmpty());      // empty again

    cout << "test_isEmpty_basic passed\n";
}

void test_assignment_deep_copy() {
    BinomialHeap<int> h1;
    h1.insert(10);
    h1.insert(5);
    h1.insert(20);

    BinomialHeap<int> h2;
    h2 = h1;

    // Modify original
    h1.extractMin();  // removes 5

    // h2 must be unchanged
    assert(h2.getMin() == 5);
    assert(h2.size() == 3);

    cout << "test_assignment_deep_copy passed\n";
}


void test_assignment_overwrites_existing_heap() {
    BinomialHeap<int> h1;
    h1.insert(1);
    h1.insert(2);

    BinomialHeap<int> h2;
    h2.insert(100);
    h2.insert(200);
    h2.insert(300);

    h2 = h1;

    assert(h2.size() == h1.size());
    assert(h2.getMin() == h1.getMin());

    cout << "test_assignment_overwrites_existing_heap passed\n";
}

void test_self_assignment() {
    BinomialHeap<int> h;
    h.insert(10);
    h.insert(2);
    h.insert(15);

    h = h;  // self-assignment

    assert(h.size() == 3);
    assert(h.getMin() == 2);

    cout << "test_self_assignment passed\n";
}


void test_size_empty() {
    BinomialHeap<int> h;
    assert(h.size() == 0);
    cout << "test_size_empty passed\n";
}
void test_size_single() {
    BinomialHeap<int> h;
    h.insert(10);
    assert(h.size() == 1);
    cout << "test_size_single passed\n";
}

void test_size_multiple() {
    BinomialHeap<int> h;
    h.insert(10);
    h.insert(5);
    h.insert(20);
    h.insert(3);

    assert(h.size() == 4);
    cout << "test_size_multiple passed\n";
}
void test_size_after_extract() {
    BinomialHeap<int> h;
    h.insert(10);
    h.insert(5);
    h.insert(20);

    h.extractMin();   // removes 5

    assert(h.size() == 2);
    cout << "test_size_after_extract passed\n";
}
void test_size_after_clear() {
    BinomialHeap<int> h;
    h.insert(1);
    h.insert(2);
    h.insert(3);

    h.clear();

    assert(h.size() == 0);
    cout << "test_size_after_clear passed\n";
}


void test_delete_existing() {
    BinomialHeap<int> h;
    h.insert(10);
    h.insert(5);
    h.insert(20);

    h.deleteKey(10);

    assert(h.size() == 2);
    assert(h.getMin() == 5);
    cout << "test_delete_existing passed\n";
}
void test_delete_min() {
    BinomialHeap<int> h;
    h.insert(1);
    h.insert(5);
    h.insert(10);

    h.deleteKey(1);

    assert(h.size() == 2);
    assert(h.getMin() == 5);
    cout << "test_delete_min passed\n";
}
void test_delete_nonexistent() {
    BinomialHeap<int> h;
    h.insert(10);

    try {
        h.deleteKey(99);
        assert(false);  // should not reach
    }
    catch (...) {
        cout << "test_delete_nonexistent passed\n";
    }
}
void demo_delete_simple() {
    BinomialHeap<int> h;
    h.insert(10);
    h.insert(5);
    h.insert(20);

    cout << "Before delete: min = " << h.getMin() << endl;
    h.deleteKey(20);
    cout << "After delete 20: min = " << h.getMin() << endl;
    cout << "Heap size = " << h.size() << endl;
}

void test_node_size_single() {
    BinomialNode<int> node(10);
    int count = 0;
    node.size(count);
    assert(count == 1);
    cout << "test_node_size_single passed\n";
}
void test_node_size_children_only() {
    BinomialNode<int> root(10);
    root.addChild(new BinomialNode<int>(20));
    root.addChild(new BinomialNode<int>(30));

    int count = 0;
    root.size(count);
    assert(count == 3);
    cout << "test_node_size_children_only passed\n";
}
void test_node_size_grandchildren() {
    BinomialNode<int> root(10);
    BinomialNode<int>* child1 = new BinomialNode<int>(20);
    BinomialNode<int>* child2 = new BinomialNode<int>(30);
    root.addChild(child1);
    root.addChild(child2);

    child1->addChild(new BinomialNode<int>(40));
    child1->addChild(new BinomialNode<int>(50));
    child2->addChild(new BinomialNode<int>(60));

    int count = 0;
    root.size(count);
    assert(count == 6);
    cout << "test_node_size_grandchildren passed\n";
}
void demo_node_size() {
    BinomialNode<int> root(10);
    root.addChild(new BinomialNode<int>(20));
    root.addChild(new BinomialNode<int>(30));

    int count = 0;
    root.size(count);
    cout << "Node size = " << count << endl;  // Expected: 3
}



int main() {
    BinomialHeap<int> h;
    //testBasicOperations();
    //testEdgeCases();
    //testVisualOperations();
    //testDecreaseKey();
    //testInsertOperations();

    //testInsertTimeComplexity();
    //testGetMinTimeComplexity();
    //testExtractMinTimeComplexity();
    //testDecreaseKeyTimeComplexity();
    //testUnionTimeComplexity();
    // 
    // 
    //test_clear_empty_heap();
    //test_clear_single_node();
    //test_clear_multiple_inserts(); 
    //test_isEmpty_basic();

    //test_assignment_deep_copy()
    //test_assignment_overwrites_existing_heap();
    //test_self_assignment();

    /*test_size_empty();
    test_size_single();
    test_size_multiple();
    test_size_after_extract();
    test_size_after_clear();*/

   /* test_delete_existing();
    test_delete_min();
    test_delete_nonexistent();
    demo_delete_simple();*/

    test_node_size_single();
    test_node_size_children_only();
    test_node_size_grandchildren();
    demo_node_size();
}