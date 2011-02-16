/**
 * Mx prototype
 */
#include <clutter/clutter.h>
#include <mx/mx.h>

static const gint WIN_W = 640;
static const gint WIN_H = 480;

typedef struct stuff_
{
    ClutterActor *group;
    ClutterActor *slider;
} Stuff;

static void key_event_cb(ClutterActor *actor, ClutterKeyEvent *event, gpointer data)
{
    Stuff *self = (Stuff *) data;
    (void) self; // unused
    switch (event->keyval)
    {
        case CLUTTER_Escape:
            clutter_main_quit();
            break;
        case CLUTTER_space:
            break;
        default:
            break;
    }
}

static void on_button_clicked(ClutterActor *actor, gpointer data)
{
    Stuff *self = (Stuff *) data;
    (void) self; // unused
    (void) actor; // unused
    g_print("clicked\n");
    g_print("slider value: %f\n", mx_slider_get_value(MX_SLIDER(self->slider)));
}

int main(int argc, char *argv[])
{
    clutter_init(&argc, &argv);

    ClutterActor *stage = NULL;
    ClutterColor black = { 0x00, 0x00, 0x00, 0xff };
    stage = clutter_stage_get_default();
    clutter_stage_set_title(CLUTTER_STAGE(stage), "Mx test");
    clutter_stage_set_color(CLUTTER_STAGE(stage), &black);
    clutter_actor_set_size(stage, WIN_W, WIN_H);

    Stuff *self = g_new0(Stuff, 1);
    ClutterLayoutManager *layout = clutter_flow_layout_new(CLUTTER_FLOW_HORIZONTAL);
    clutter_flow_layout_set_row_spacing(CLUTTER_FLOW_LAYOUT(layout), 4.0f);
    clutter_flow_layout_set_column_spacing(CLUTTER_FLOW_LAYOUT(layout), 4.0f);
    self->group = clutter_box_new(layout);
    clutter_actor_set_width(self->group, clutter_actor_get_width(stage));
    clutter_container_add_actor(CLUTTER_CONTAINER(stage), self->group);
    clutter_actor_set_y(self->group, 8.0f);
    
    // TOOLBAR
    ClutterActor *toolbar = mx_toolbar_new();
    mx_toolbar_set_has_close_button(MX_TOOLBAR(toolbar), FALSE);
    clutter_actor_set_width(toolbar, WIN_W);
    ClutterActor *tools = mx_box_layout_new();
    ClutterActor *button_a = mx_button_new_with_label("Click A!");
    ClutterActor *button_b = mx_button_new_with_label("Click B!");
    mx_box_layout_add_actor(MX_BOX_LAYOUT(tools), button_b, 0);
    mx_box_layout_add_actor(MX_BOX_LAYOUT(tools), button_a, 0);
    mx_bin_set_child(MX_BIN(toolbar), tools), 
    clutter_container_add_actor(CLUTTER_CONTAINER(self->group), toolbar);

    // RECTANGLES
    int i;
    ClutterColor orange = { 0xff, 0xcc, 0x33, 0xff };
    for (i = 0; i < 10; ++i)
    {
        ClutterActor *actor = clutter_rectangle_new_with_color(&orange);
        gfloat SIZE = 80.0f;
        clutter_actor_set_size(actor, SIZE, SIZE);
        clutter_container_add_actor(CLUTTER_CONTAINER(self->group), actor);
    }

    // SLIDER + BUTTON
    ClutterActor *slider = mx_slider_new();
    ClutterActor *button = mx_button_new_with_label("Click me!");
    clutter_container_add_actor(CLUTTER_CONTAINER(self->group), slider);
    clutter_container_add_actor(CLUTTER_CONTAINER(self->group), button);
    self->slider = slider;
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), self);

    // DONE
    g_signal_connect(stage, "key-press-event", G_CALLBACK(key_event_cb), self);
    clutter_actor_show(stage);
    clutter_main();
    return 0;
}

