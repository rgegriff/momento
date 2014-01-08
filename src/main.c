#include <pebble.h>
	
Window *my_window;
int32_t HOUR_ANGLE;
int32_t MINUTE_ANGLE;
int32_t SECOND_ANGLE;
int WIDTH  = 144;
int HEIGHT = 168;
//int STATBAR= 16;

GPoint get_endpoint(GPoint center, int length, int32_t angle){
	int32_t y = (-cos_lookup(angle) * length / TRIG_MAX_RATIO) + center.y;
	int32_t x = (sin_lookup(angle) * length / TRIG_MAX_RATIO) + center.x;
	return GPoint(x,y);
}

void draw_circle(Layer *layer, GContext *gctxt){
	GPoint circ_center = GPoint(WIDTH/2, (HEIGHT/2));
    //Emblacken the screen
	graphics_context_set_fill_color(gctxt, GColorBlack); graphics_fill_rect(gctxt, GRect(0,0,WIDTH,HEIGHT), 0, GCornerNone); 
	//Draw outer circle 
	graphics_context_set_fill_color(gctxt, GColorWhite);
	graphics_fill_circle(gctxt, circ_center, (WIDTH/2)-8);
	//Draw inner circle
	graphics_context_set_fill_color(gctxt, GColorBlack);
	graphics_fill_circle(gctxt, circ_center, (WIDTH/4));
	//Draw hour line
	graphics_context_set_stroke_color(gctxt, GColorBlack);
    graphics_draw_line(gctxt, get_endpoint(circ_center, (WIDTH/4), MINUTE_ANGLE), get_endpoint(circ_center, (WIDTH/2), MINUTE_ANGLE));
    graphics_draw_line(gctxt, get_endpoint(circ_center, (WIDTH/4), SECOND_ANGLE), get_endpoint(circ_center, (WIDTH/2), SECOND_ANGLE));
	
	
	graphics_context_set_stroke_color(gctxt, GColorWhite);
	graphics_draw_line(gctxt, circ_center, get_endpoint(circ_center, (WIDTH/4), HOUR_ANGLE));
	
	//graphics_context_set_stroke_color(gctxt, GColorBlack);
	//int32_t angle = (TRIG_MAX_ANGLE * 4 / 12);
	//GPoint start;
	//GPoint end;
	//int32_t c = 0;
	//while(c!=12){
	//	angle = (TRIG_MAX_ANGLE * c / 12);	
	//   start = get_endpoint(circ_center, (WIDTH/6)+33, angle);
	//    end = get_endpoint(circ_center, (WIDTH/2), angle);
		//graphics_draw_line(gctxt, start, end);
	//	c = c + 3;
	//}
}

void tick_handler(struct tm *tick_time, TimeUnits units_changed){
	struct tm tm = *tick_time;
	SECOND_ANGLE = TRIG_MAX_ANGLE * tm.tm_sec / 60;
    MINUTE_ANGLE = TRIG_MAX_ANGLE * tm.tm_min / 60;
	HOUR_ANGLE = (TRIG_MAX_ANGLE * tm.tm_hour / 12) + (((TRIG_MAX_ANGLE/12)/60) * tm.tm_min);
	layer_mark_dirty(window_get_root_layer(my_window));

}

void window_load(Window *window){
    layer_set_update_proc(window_get_root_layer(my_window), draw_circle);
}

void window_unload(Window *window){
	//text_layer_destroy(text_layer);
	//layer_destroy(Layer* layer)
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
