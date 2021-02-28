#include "LineMessage.h"


LineMessage::LineMessage(String Host, String Linetonken)
{
    host = Host;
    LineTonken = Linetonken;
}

LineMessage::~LineMessage()
{
}

void LineMessage::setMessage(String Message)
{
    message = Message;
}
void LineMessage::sendMessage()
{
    char char_host[host.length() + 1];
    host.toCharArray(char_host, host.length() + 1);
    Serial.println(message);
    if (client.connect(char_host, 443)) {
		int LEN = message.length();
		//  POST
		String url = "/api/notify";
		client.println("POST " + url + " HTTP/1.1");
		client.print("Host: "); client.println(host);
		// Tonken
		client.print("Authorization: Bearer "); client.println(LineTonken);
		client.println("Content-Type: application/x-www-form-urlencoded");
		client.print("Content-Length: "); client.println( String((LEN + 8)) );
		client.println();      
		client.print("message="); client.println(message);
		client.println();
		// Watting response
		// delay(2000);
		String response = client.readString();
		//Display response
		Serial.println(response);
		client.stop(); //Disconnect response
    }
    else 
	{
		// Error
		Serial.println("connected fail");
    }
    
}
