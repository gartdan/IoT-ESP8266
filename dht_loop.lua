

	local PIN = 4
	local DHT= require("dht_lib")
	DHT.read11(PIN)
	
	temp = DHT.getTemperature()
	humidity = DHT.getHumidity()
	
	print("Temp:"..temp)
	tF = (temp * 9/5) + 32
	print ("TempF: "..tF)
	print("Humidity:"..humidity.."%")
		
	if temp == nil then
			print("Value is null, try again")
		else
			print("Sending data to thingspeak.com")
			conn=net.createConnection(net.TCP, 0)
			conn:on("receive", function(conn, payload) print(payload) end)
			conn:connect(80,'184.106.153.149')
			conn:send("GET /update?key=P230ZZ0M65LQRXG2&field1="..tF.."&field2="..temp.."&field3="..humidity.." HTTP/1.1\r\n")
			conn:send("Host: api.thingspeak.com\r\n")
			conn:send("Accept: */*\r\n")
			conn:send("User-Agent: Mozilla/4.0 (compatible; esp8266 Lua; Windows NT 5.1)\r\n")
			conn:send("\r\n")
			conn:on("sent",function(conn) print("Closing connection") conn:close() end)
			conn:on("disconnection", function(conn) print("Got disconnection...") end)
			conn = nil
	end







