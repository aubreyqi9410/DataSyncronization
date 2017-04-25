#include "testglove.h"
#include "UDPclient.h"


int main() {
	if (openGlove() == 0) {
		startUDPClient();
		closeGlove();
	}
	return 0;
}