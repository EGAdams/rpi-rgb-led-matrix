Here is the psudo code that I want you to follow:
```psuedo
open test configuration file and read the names of the classes that need to be constructed for the tests.

for each class in the list of classes
    create an instance of the class using the name string from the configuration file
    call the execute method on the instance
    delete the instance
    if the test failed
        print the name of the class and the error message
    end if
end for
```

sample config file
```config
TestMode1P1Score_4Points
TestTieBreakWinBy6Scenario
TestMatchWinScenario
TestWinBy2Scenario
```

Then we would need a way to convert these strings to actual objects in C
++

Please use the GoF Design Patterns in cases where it would make the design and code cleaner.

Please begin writting the C++ code.  I wan't you to focus more on the code rather that focusing on the explaination.  We will have plenty of time to document the code later.
