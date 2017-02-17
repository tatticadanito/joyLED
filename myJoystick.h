#include <stdio.h>
#include <stdlib.h>
#include <linux/joystick.h>
#include <fcntl.h>

int open_joystick(char *device_name);	//Opens device
int read_event(int joystick_fd, struct js_event *jse);	//Reads device's event
int is_good_analog(struct js_event *jse);	//Checks if is valid analog
int is_front_button(struct js_event *jse);	//Checks if is valid button (back && front)

int open_joystick(char *device_name)
{
	int joystick_fd = -1;

	if(device_name == NULL)
		return joystick_fd;

	joystick_fd = open(device_name, O_RDONLY | O_NONBLOCK);

	if(joystick_fd < 0) {
		printf("Could not locate joystick device %s\n", device_name);
		return joystick_fd;
	}

	return joystick_fd;
}

int read_event(int joystick_fd, struct js_event *jse)
{
	int bytes;

	bytes = read(joystick_fd, jse, sizeof(*jse));

	if (bytes == -1) return 0;
	if (bytes == sizeof(*jse)) return 1;

	printf("Inexpected bytes from joystick:%d\n", bytes);

	return -1;
}

int is_good_analog(struct js_event *jse)
{
	if(jse->type == 2 && jse->value == 32767 || jse->value == -32767 || jse->value == 0) return 1;
	else return 0;
}

int is_good_button(struct js_event *jse)
{
	if(jse->number <10 && jse->value == 1 || jse->value == 0 && jse->type == 1 && jse->number < 8) return 1;
	else return 0;
}
