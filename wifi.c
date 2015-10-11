
/*
void setupWiFi()
{
	restartWiFi();

	boolean connected = false;
	while (!connected)
	{
		if (connectWiFi())
		{
			connected = true;
			break;
		}
	}
	delay(2000);

	printer("AT+CIPMUX=0");
}

void restartWiFi()
{
	while (!Serial)
		;
	while (!esp8266)
		;

	while (esp8266.available() > 0)
	{
		esp8266.read();
	}

	esp8266.println("AT");

	boolean reset = true;
	while (reset)
	{
		esp8266.flush();
		esp8266.println("AT+RST");
		if (esp8266.find("Ready") || esp8266.find("ready"))
		{
			Serial.println("Module is ready");
			reset = false;
		}
		else
		{
			Serial.println(esp8266.read());
			Serial.println("Module have no response.");
		}
	}

	delay(1000);
}

void updateRemoteOptoState(boolean state)
{
	String valueToSend = state ? "true " : "false";
	long length = state ? 65 : 66;

	String cmd = "AT+CIPSTART=\"TCP\",\"";
	cmd += DST_IP;
	cmd += "\",80";
	esp8266.println(cmd);
	Serial.println(cmd);
	if (esp8266.find("Error")) return;
	cmd = apiRequest(valueToSend);
	String cip_cmp = "AT+CIPSEND=";
	cip_cmp += length;
	printer(cip_cmp);
	while (!esp8266.find(">"))
	{
		Serial.println("No response...");
		
		reset();
	}

	printer(cmd);
	printer("host:domain.com");
	printer("");
	printer("");
	delay(1500);

	while (esp8266.available())
	{
		char c = esp8266.read();
		Serial.write(c);
		if (c == '\r') Serial.print('\n');
	}
	Serial.println("====");
}

String apiRequest(String valueToSend)
{
	String cmd = "GET /toxic.php?room=";
	cmd += roomNumber;
	cmd += "&status=";
	cmd += valueToSend;
	cmd += " HTTP/1.0";
	return cmd;
}

boolean connectWiFi()
{
	esp8266.println("AT+CWMODE=1");
	String cmd = "AT+CWJAP=\"";
	cmd += SSID;
	cmd += "\",\"";
	cmd += PASSWORD;
	cmd += "\"";
	printer(cmd);
	delay(3000);
	if (esp8266.find("OK"))
	{
		Serial.println("OK, Connected to WiFi.");
		return true;
	}
	else
	{
		Serial.println("Can not connect to the WiFi.");
		return false;
	}
}

void reset()
{
	Serial.println("Reset");
	digitalWrite(resetPin, LOW);
}

void printer(String cmd)
{
	Serial.println(cmd);
	esp8266.println(cmd);
}
*/