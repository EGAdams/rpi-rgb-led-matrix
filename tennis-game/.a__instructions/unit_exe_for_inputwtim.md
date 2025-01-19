I test individual classes a little differently than the standards may suggest.  I like to create one executable file with the class to be tested inside of that executable file.  In order to run the test on one class, one has to just run this executable to test this one class.  This way we avoid having to use unit test frameworks.  Please write an executable to test this concrete InputWithTimer class that is being used in the run_remote_listener method that you have described before.  Here is the code: ```cpp // Example concrete InputWithTimer
class InputWithTimer : public IInputWithTimer {
public:
    InputWithTimer( std::shared_ptr<IPairingBlinker> blinker,
                    std::shared_ptr<IInputs> inputs )
        : m_blinker( blinker ), m_inputs( inputs ), m_timeSlept( 0 ) {}

    int getInput() override {
        // For a real implementation, you'd start a timer thread,
        // watch for input, etc. We'll keep it simple:
        using namespace std::chrono;

        auto start = steady_clock::now();
        int value = 0;
        if ( REMOTE_INPUT == 1 ) {
            // Suppose we read from the inputs interface
            value = m_inputs->read_mcp23017_value();
        } else {
            std::cout << "Enter a value: ";
            std::cin >> value; // local
        }
        auto end = steady_clock::now();

        m_timeSlept = duration_cast<milliseconds>( end - start ).count();
        return value;
    }

    long getTimeSlept() const override {
        return m_timeSlept;
    }

private:
    std::shared_ptr<IPairingBlinker> m_blinker;
    std::shared_ptr<IInputs>         m_inputs;
    long                             m_timeSlept;
}; 
```  
Just put your output on one code block.  I will g++ compile it an run the test.  Don't forget to add a bunch of edge cases.  I want this thing tested very thoroughly.