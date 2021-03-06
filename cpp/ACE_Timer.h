//============================================================
/**
 * @file Timer.h
 *
 * taken from basic sp timer code
 * @author
 */
//============================================================

#ifndef TIMER_H
#define TIMER_H

#include "ace/Task.h"

/**
  * @class Timer
  *
  */
class Timer : public ACE_Task_Base
{
public:
  /// Default constructor.
  Timer (void);

  /// Default destructor.
  ~Timer (void);

  long int hertz (void);
  void hertz (long int hertz);
  void start ();
  void stop ();

  /// Helper function to be called back after a timeout
  virtual int pulse (void);

  ///  timer related functions including the active object svc function which
  ///  runs our timer

  /// Handle the timeout.
  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *arg);

  /// Called when <Time_Handler> is removed.
  virtual int handle_close (ACE_HANDLE handle,
                            ACE_Reactor_Mask close_mask);

  virtual int svc (void);

protected:
  /// Flag to indicate completion of this active object.
  int done_;

  /// flag to indicate if we are already activated
  int active_;

  /// Frequency
  long int hertz_;

  /// The timer id we are waiting.
  long tid_;

};



#endif /* TIMER_H */
