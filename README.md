# TetrisGame

**TetrisGame** is a Win32 application implemented in C++ using modern threading and Direct2D/DirectComposition for rendering. It showcases a clean separation between the UI (main thread) and the game logic (worker thread), providing smooth animations, responsive controls, and efficient CPU usage.

---

## Features

* **Multithreaded Game Loop**:

  * Commands (move, rotate, hard drop, pause) are enqueued by the UI thread and processed by a dedicated logic thread.
  * Gravity ticks occur every 500 ms independently of UI updates.
* **Responsive Input Handling**:

  * Input commands are executed immediately, without blocking the main rendering thread.
  * Fast left/right movement and hard drop animations.
* **Efficient CPU Usage**:

  * Uses `std::condition_variable::wait_for` to sleep until the next event (command or gravity), minimizing busy-wait.
* **Pause Functionality**:

  * Atomic flag controls pause state; logic loop skips game updates while paused.
* **Direct2D & DirectComposition Rendering**:

  * Hardware-accelerated rendering with D3D11 + D2D1 + DComposition.
* **Clean Architecture**:

  * `TetrisWindow` class handles window messages, resource initialization, and delegates game updates to `LogicLoop()`.

---

## Code Structure

```
TetrisWindow.h             // Public interface and member declarations
TetrisWindow.cpp           // Message handling, rendering, and logic thread
GameField.h/.cpp           // Core game state and tetromino management
Renderer.hpp/.cpp          // Drawing helper for game field and UI
ResourceManager.h/.cpp     // WIC & DWrite resource initialization
Button.h/.cpp              // UI button abstraction
Constants.h                // Layout, colors, font sizes
GameOverWindow.h/.cpp      // End-of-game popup and restart logic
```

---

## LogicLoop Overview

```cpp
void TetrisWindow::LogicLoop() {
    static constexpr auto gravityInterval = std::chrono::milliseconds(500);
    auto nextGravity = std::chrono::steady_clock::now() + gravityInterval;
    std::deque<Command> commandBuffer;

    while (m_logicRunning) {
	    // 1) Sleep until either: new command arrives OR gravity tick
	    auto now = std::chrono::steady_clock::now();
	    auto waitTime = (now < nextGravity) ? nextGravity - now : std::chrono::milliseconds(0);
	    {
	        std::unique_lock<std::mutex> lock(m_cmdMutex);
	        m_cmdCV.wait_for(lock, waitTime, [&] { return !m_logicRunning || !m_commands.empty(); });
	        commandBuffer.swap(m_commands);
	    }
	    // 2) Handle pause state
	    if (m_isPaused.load(std::memory_order_relaxed)) {
	        nextGravity = std::chrono::steady_clock::now() + gravityInterval;
            PostMessage(m_window, WM_APP_REDRAW, 0, 0);
	        std::this_thread::sleep_for(std::chrono::milliseconds(16));
	        continue;
	    }
	    // 3) Execute all pending commands
	    for (const auto& cmd : commandBuffer) {
            ExecuteCommand(cmd);
	    }
	    
	    bool changed = !commandBuffer.empty();
	    commandBuffer.clear();
	    now = std::chrono::steady_clock::now();
	    // 4) Gravity step if interval elapsed
	    if (now >= nextGravity) {
            nextGravity = now + gravityInterval;
	        changed |= GravityStep();
	    }
	    // 5) Request redraw when state changed
	    if (commandBuffer.empty()) {
	        PostMessage(m_window, WM_APP_REDRAW, 0, 0);
	    }
    }
}
```

* **Command Processing**: Commands are queued under `m_cmdMutex` and drained into a local `commandBuffer`.
* **Pause Check**: Atomic `m_isPaused` flag ensures minimal overhead to skip updates.
* **GravityStep**: Moves the current tetromino down or locks it in place.

---

## Building & Running

1. **Prerequisites**:

   * Windows 10 or later
   * Visual Studio 2019+ with C++ Desktop Development workload
2. **Clone the repo**:

   ```bash
   git clone https://github.com/username/TetrisGame.git
   ```
3. **Open** `TetrisGame.sln` in Visual Studio.
4. **Build** the solution (Debug or Release).
5. **Run** `TetrisGame.exe` from the output folder.

---

## License

MIT © Your Name
