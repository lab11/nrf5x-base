#pragma once

typedef struct {
  uint8_t   channel;
  uint16_t  panid;
  bool      sed; // sleepy end device
  uint32_t  poll_period;
  uint32_t  child_period;
  bool      autocommission;
} thread_config_t;

void thread_state_changed_callback(uint32_t flags, void * p_context);

void thread_init(const thread_config_t* config);
void thread_process(void);
void thread_sleep(void);
otInstance * thread_get_instance(void);
