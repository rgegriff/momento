#include <pebble.h>

Window *my_window;
TextLayer *text_layer;
GContext *g_context;
int32_t WIDTH  = 144;
int32_t HEIGHT = 168;
int32_t HOUR_ANGLE;
int32_t MINUTE_ANGLE;

//GPoint CENTER_WATCH = GPoint(WIDTH/2, HEIGHT/2);
//GPoint CENTER_APP   = GPoint(WIDTH/2, (HEIGHT/2)+8); //Translate down 8px. Find out of this is right.

GPoint get_endpoint(GPoint center, int length, int32_t angle){
	int32_t y = (-cos_lookup(angle) * length / TRIG_MAX_RATIO) + center.y;
	int32_t x = (sin_lookup(angle) * length / TRIG_MAX_RATIO) + center.x;
	return GPoint(x,y);
}

void draw_circle(Layer *layer, GContext *gctxt){
	GPoint circ_center = GPoint(WIDTH/2, (HEIGHT/2)+12);
	//Draw hour circle
	graphics_context_set_fill_color(gctxt, GColorBlack);
	graphics_fill_circle(gctxt, circ_center, (WIDTH/2)-8);
	//Draw minute circle
	graphics_context_set_fill_color(gctxt, GColorWhite);
	graphics_fill_circle(gctxt, circ_center, (WIDTH/6));
	
	graphics_context_set_stroke_color(gctxt, GColorWhite);
	graphics_draw_line(gctxt, circ_center, get_endpoint(circ_center, (WIDTH/2)-8, HOUR_ANGLE));

	graphics_context_set_stroke_color(gctxt, GColorBlack);
	graphics_draw_line(gctxt, circ_center, get_endpoint(circ_center, (WIDTH/2)-8, MINUTE_ANGLE));
}

void tick_handler(struct tm *tick_time, TimeUnits units_changed){
	//strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
	struct tm tm = *tick_time;
    MINUTE_ANGLE = TRIG_MAX_ANGLE * tm.tm_min / 60;
	HOUR_ANGLE = TRIG_MAX_ANGLE * tm.tm_hour / 12;
	layer_mark_dirty(window_get_root_layer(my_window));

}

void window_load(Window *window){
	text_layer = text_layer_create(GRect(0, 0, 144, 20));
	text_layer_set_text(text_layer, "Even you will die.");
	text_layer_set_background_color(text_layer, GColorClear);
	text_layer_set_text_color(text_layer, GColorBlack);
	//text_layer_set_size(text_layer, GSize(5));
	text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
	layer_add_child(window_get_root_layer(my_window), (Layer*) text_layer);
    layer_set_update_proc(window_get_root_layer(my_window), draw_circle);

	//grahics_context_set_stroke_color(g_context, GColorBlack);
	
}

void window_unload(Window *window){
	text_layer_destroy(text_layer);
}
                                                             
void handle_init(void) {
	tick_timer_service_subscribe(SECOND_UNIT, (TickHandler) tick_handler);
	my_window = window_create();
	window_set_window_handlers(my_window, (WindowHandlers){
		.load = window_load,
		.unload = window_unload,
	});
	window_stack_push(my_window, true);
}

void handle_deinit(void) {
	window_destroy(my_window);
	tick_timer_service_unsubscribe();
}

int main(void) {
	  handle_init();
	  app_event_loop();
	  handle_deinit();
}
