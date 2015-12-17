#include "filter.h"
#include "enet.h"

const byte IP_BING[4] = {204, 79, 197, 200};

void Filter_Reset(uint packet) {
	if (NET_Packet[packet].Payload[9] == 0x6) {
		NET_Packet[packet].Payload[33] |= 0x4;
	}
}

//---------------------------------------------------------------------------------------+
// Validate all IPv4 packets. Block packets with the same ip address as bing             |
// Note: This method may block undesired domains hosted on the same server               |
// (This doesn't appear to be a problem with bing.com, but it may for other domains)     |
//---------------------------------------------------------------------------------------+
bool Filter_IP(uint packet) {
	if (NET_Packet[packet].Type == 0x0800) { // IPv4
		// Beginning of Destination in IPv4 header is index 16
		if (NET_Packet[packet].Payload[16] == IP_BING[0]) {
			if (NET_Packet[packet].Payload[17] == IP_BING[1]) {
				if (NET_Packet[packet].Payload[18] == IP_BING[2]) {
					if (NET_Packet[packet].Payload[19] == IP_BING[3]) {
						return true;
					}
				}
			}
		}
	}
	
	return false;
}

//---------------------------------------------------------------------------------------+
// Validate only DNS packets. Check for requests for bing's IP address                   |
// Note: If the client computer has the ip cached, this won't block a page load          |
//---------------------------------------------------------------------------------------+
bool Filter_DNS(uint packet) {
	uint i;
	
	if (NET_Packet[packet].Type == 0x0800) { // IPv4
		if (NET_Packet[packet].Payload[9] == 0x11) { // UDP flag in IPv4 header
			i = 28; // Skip the IPv4 Header and the UDP header
			// no need to check for 'b' in 'bing.com' in last 7 bytes.
			// Also, DNS Query: the last 4 bytes signify the class and type
			while(i < NET_Packet[packet].PayloadSize - 11) {
				if (NET_Packet[packet].Payload[i] == 'b') {
					if (NET_Packet[packet].Payload[++i] == 'i') {
						if (NET_Packet[packet].Payload[++i] == 'n') {
							if (NET_Packet[packet].Payload[++i] == 'g') {
								return true;
							}
						}
					}
				}
			};
		}
	}
	
	return false;
}

//---------------------------------------------------------------------------------------+
// Validate all packets. Indiscriminately search for 'bing' anywhere in the packet       |
// Note: Any filtering will reduce overal internet speed; but this is the slowest        |
//---------------------------------------------------------------------------------------+
bool Filter_Deep(uint packet) {
	uint i = 0;
	
	while(i < NET_Packet[packet].PayloadSize - 5) {
		if (NET_Packet[packet].Payload[i] == 'b') {
			if (NET_Packet[packet].Payload[++i] == 'i') {
				if (NET_Packet[packet].Payload[++i] == 'n') {
					if (NET_Packet[packet].Payload[++i] == 'g') {
						return true;
					}
				}
			}
		}
	};
	
	return false;
}