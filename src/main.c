#include <pebble.h>

Window *my_window;

static void tick_handler(struct tm *tick_time, TimeUnits changed) {
	// 5minute
  int minuts = tick_time->tm_min;
  if(minuts%5 == 0){
    vibes_short_pulse();
  }
}

void handle_init(void) {
  my_window = window_create();

  #ifdef PBL_SDK_3
  window_set_background_color(my_window, GColorRed);
  #endif	
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  window_stack_push(my_window, true);
}

void handle_deinit(void) {
  window_destroy(my_window);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
