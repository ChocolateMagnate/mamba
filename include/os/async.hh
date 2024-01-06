/*+===================================================================
  SyncFile:    async.hh

  Summary:     Implements the Mambic event loop and asynchronous programming
               in Python.

  Classes:     Coroutine

  Functions:

  Available under Apache Licence v2. Mamba Authors (2023)
===================================================================+*/
#pragma once

#include <cstdlib>
#include <vector>
#include <bitset>

#include "types/Object.hh"

namespace os {
  const auto DefaultNumberOfTasks = 1;
  typedef size_t EventHandle;

  /// Represents events that signal readability of an asynchronous task. They are used
  /// to perform syscalls that would wake the Python's process in response to an action.
  enum class EventListener {
    FileReadability, FileWritability,
  };

  /// Represents the asynchronous tasks scheduled and executed by the event loop.
  /// This is the return value of async routines and they are set in action with await.
  struct Coroutine {
    size_t id = 0;
    EventListener listener{};
    bool pending = true;
  };

  /// The interface for event loop used to schedule and await coroutines. This header
  /// file contains the essential contract that asynchronous scheduler must implement
  /// as higher-level abstraction for callers.
  class EventLoop {
   public:
    EventLoop(const size_t numberOfTasks = DefaultNumberOfTasks);

    //Event loops is a singleton and can only run on a single thread.
    EventLoop(const EventLoop&) = delete;
    EventLoop(const EventLoop&&) = delete;
    EventLoop& operator=(const EventLoop&) = delete;
    EventLoop& operator=(const EventLoop&&) = delete;

    /// Adds an event asynchronous task into the queue. This method must be
    /// thread-safe as it can be called from multiple threads, while there is
    /// only one event loop at any given moment.
    /// @param task The task to add to the scheduler
    /// @return Reference to self.
    EventLoop& post(Coroutine& task);

    /// Listens to a specific event associated with an event success.
    /// @param listener The type of event to listen to.
    /// @return The handle for the event used to remove the listening.
    EventHandle addEventListener(const EventListener listener);

    /// Removes an event listener for a specific event.
    /// @param handle The handle for the listening syscall returned by addEventListener.
    void removeEventListener(const EventHandle handle);

   private:
    std::bitset<2> eventListeners;
    std::vector<Coroutine> tasks;
  };

  /// Enters an event loop and suspends the main thread until the task is complete.
  /// Meanwhile, other tasks can be scheduled and executed with aid of non-blocking
  /// system calls, and subsequent calls to await would return early as other tasks
  /// would already be awaited.
  /// @param task The coroutine object to await for. If it was already scheduled and
  /// awaited earlier, it would return instantly, otherwise it would begin an event
  /// loop and schedule other tasks while this one is running.
  /// @return A Pythonic object whatever the task resolves to.
  mamba::Object await(Coroutine& task);
}