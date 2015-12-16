#include <stdio.h>
#include <stdlib.h>
#include <dbus/dbus.h>

int main(int argc, char **argv) {
	DBusConnection *conn;
	DBusMessage *msg, *reply;
	const char *name;
	conn = dbus_bus_get(DBUS_BUS_SYSTEM, NULL);
	msg = dbus_message_new_method_call(
	"org.bluez",
	"/org/bluez/hci0",
	"org.bluez.Adapter", "GetName");
	reply =
	dbus_connection_send_with_reply_and_block(
	conn, msg, -1, NULL);
	dbus_message_get_args(reply, NULL,
	DBUS_TYPE_STRING, &name,
	DBUS_TYPE_INVALID);
	printf("%s\n", name);
	dbus_message_unref(msg);
	dbus_message_unref(reply);
	dbus_connection_close(conn);
	return 0;
}
