# Persona
You are a world-class C++ Developer with 30 years of experience in designing and implementing complex Tennis game Scoreboard systems.

# Your task
I am trying to write clear and concise instructions for a C++ developer to implement the logic flow of a Tennis game.  I need you to analyze, heavily scrutinize and rewrit the following instructions to be more clear and concise.  You should also add any additional information that you think is necessary to make the instructions clear and concise.

```markdown
# Detailed Logic Flow
1. The game starts in Pairing Mode, waiting for the Green Player score and then the Red Player score. 
2. If the system is in Pairing Mode for over 10 seconds, the game should go to Sleep Mode.
    - In Sleep Mode, the game will blink a light to indicate that the game is in Sleep Mode.
3. An input of Green Player score, Red Player score or Undo will take the system out of Sleep Mode.
4. When it breaks out of Sleep Mode, the game will go to Regular Game Play Before any Scores.
5. If the system is in Regular Game Play Before any Scores for over 10 seconds, the game should go back to Sleep Mode.
6. If a score is made, the game will go to Regular Game Play and there are no timers after that.
7. After a full game is played and the Match is won, the system should go to Sleep Mode.  If the Green Player or Red Player score pressed within 10 seconds of being in this Sleep Mode, the game will go to Regular Game Play but retain game history enabling the Undo button to work.  
8. If Undo is pressed after breaking out of sleep mode, it should take the system back one point before the Match win.
9. If the system is in Sleep Mode for over 10 seconds and a button is pressed, the game will go to Regular Game Play, but the game history will be cleared and Undo will not take us back to the point before the Match win.
```

Please rewrite the instructions to be more clear and concise.  Put your answer in one markdown code block.

answer: https://chatgpt.com/share/678d2d15-5610-8006-8e2f-74640f83d810

# Logic Flow Instructions

1. **Game Start**: The game begins in Pairing Mode, awaiting Green Player and Red Player scores.
2. **Sleep Mode Activation**:
   - If Pairing Mode lasts over 10 seconds, the system enters Sleep Mode and blinks a light to indicate this state.
3. **Exiting Sleep Mode**:
   - Any input (Green Player score, Red Player score, or Undo) exits Sleep Mode.
   - After exiting Sleep Mode, the system transitions to Regular Game Play (Before Scores).
4. **Timeout to Sleep**:
   - If Regular Game Play (Before Scores) is idle for 10 seconds, the system re-enters Sleep Mode.
5. **Scoring Transition**:
   - A score transitions the game to Regular Game Play, after which no timers are active.
6. **End of Match**:
   - After a Match win, the system enters Sleep Mode.
   - If a Green or Red Player score is pressed within 10 seconds of this Sleep Mode, the game resumes Regular Game Play, retaining game history to enable Undo.
7. **Undo After Match Win**:
   - Pressing Undo after exiting Sleep Mode takes the game back one point before the Match win.
8. **Cleared History**:
   - If Sleep Mode lasts over 10 seconds, any input transitions the system to Regular Game Play, but game history is cleared, and Undo cannot revert to the pre-Match state.

