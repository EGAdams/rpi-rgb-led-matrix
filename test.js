class Test {
    constructor(mapper) {
      this.mapper = mapper;
    }
  
    run() {
      const testCases = [
        { visible: { x: 0, y: 0 }, expected: { x: 0, y: 0 } },
        { visible: { x: 5, y: 10 }, expected: { x: 5, y: 5 } },
        { visible: { x: 12, y: 27 }, expected: { x: 12, y: 18 } },
        // Add more test cases as needed
      ];
  
      let passed = 0;
      let failed = 0;
  
      testCases.forEach(testCase => {
        const result = this.mapper.mapVisibleToInternal(testCase.visible.x, testCase.visible.y);
        if (JSON.stringify(result) === JSON.stringify(testCase.expected)) {
          passed++;
        } else {
          failed++;
          console.error(`Test failed for visible (${testCase.visible.x}, ${testCase.visible.y}). Expected: (${testCase.expected.x}, ${testCase.expected.y}), got: (${result.x}, ${result.y})`);
        }
      });
  
      console.log(`Test results: ${passed} passed, ${failed} failed.`);
    }
  }
  
  function runTest() {
    const mapper = new SimpleMultiplexingMapper();
    const test = new Test(mapper);
    test.run();
  }
  
  function setupTestButton() {
    const testButton = document.getElementById("run-test-button");
    testButton.addEventListener("click", runTest);
  }
  
  setupTestButton();
  