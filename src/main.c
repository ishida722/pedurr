#include <pebble.h>

#define WAKEUP_RESON 0
#define WAKEUP_ID_KEY 0
#define PERSIST_TIME 1
#define RAND_MAX 63

Window *my_window;
static WakeupId s_wakeup_id;

static void set_BG_color(void)
{
	srand(time(NULL));
	int color = 255 - rand();
	window_set_background_color(my_window, (GColor8){.argb = color});
}

static void tick_handler(struct tm *tick_time, TimeUnits changed) {
	// 5minute
	/* int minuts = tick_time->tm_min; */
	time_t now = time(NULL);
	time_t timestamp = persist_read_int(PERSIST_TIME);
	if(now - timestamp > 300){
		vibes_short_pulse();
		persist_write_int(PERSIST_TIME, now);
	}
	set_BG_color();
	window_stack_push(my_window, false);
}

static void wakeup_handler(WakeupId id, int32_t reson)
{
	vibes_short_pulse();
	time_t timestamp = time(NULL) + 10;
	s_wakeup_id = wakeup_schedule(timestamp, WAKEUP_RESON, true);
	persist_write_int(WAKEUP_ID_KEY, s_wakeup_id);
}

void handle_init(void) {
	my_window = window_create();

#ifdef PBL_SDK_3
	/* window_set_background_color(my_window, GColorRed); */
	set_BG_color();
#endif
	tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
	if(!persist_exists(PERSIST_TIME)){
		persist_write_int(PERSIST_TIME, time(NULL));
	}

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
