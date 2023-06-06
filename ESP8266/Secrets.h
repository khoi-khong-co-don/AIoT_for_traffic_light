#include <pgmspace.h>
 
#define SECRET
 
const char WIFI_SSID[] = "Ớ___KIMOCHI";               //TAMIM2.4G
const char WIFI_PASSWORD[] = "emmetemsuongchua";           //0544287380
 
#define THINGNAME "ESP8266"
 
int8_t TIME_ZONE = -5; //NYC(USA): -5 UTC
 
const char MQTT_HOST[] = "a2b5fvitucnvrx-ats.iot.ap-southeast-1.amazonaws.com";
 
 
static const char cacert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";
 
 
// Copy contents from XXXXXXXX-certificate.pem.crt here ▼
static const char client_cert[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWjCCAkKgAwIBAgIVAKLRTIgGzwZIPH0Wo6qqR8d4G3vhMA0GCSqGSIb3DQEB
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yMzA0MDYxNTIx
NDFaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh
dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDlc5R+h6xM5jyzmk17
dfrxBl8ahJ30J8S+P9U/Maubo044Re+EjEpwJMfDWn7jVaCV8m57rjC5GuZ11M1v
GRwB5xnlLlwD1VtK0hznYc0IQ99gOfGpFTCzzBKZ9tjnjmIon4T3SLOaqAz4efsk
JN6cqsxAE7s745boL4cGmtb52efYxfLCXM0KQGSZSP2NcmsEGhEj12rAvgmIHSzH
64Vt07z/P6+qoCs2KH8j6ifLELiYtM54bOOlsH1A5kaPP9D/j9Jnxdh3ypsBS16e
8pAgFnxjgIvOsb151hd3+Lhtr9hvdJvhNibG6HtQk26Jb5Vq5O+yxBhthMeGFr3b
3EYLAgMBAAGjYDBeMB8GA1UdIwQYMBaAFG18Y6rvn/dF/2R0BHcVaXKdRncIMB0G
A1UdDgQWBBRSXkkcsAdcN37+MS5yyvQVmPpnADAMBgNVHRMBAf8EAjAAMA4GA1Ud
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEASx9X3kcgJHKcq7gc40gxc/eQ
rYhH0Kr52ljevpxUeDbnqtGEAyBfB6yGe01U3TICDrUPco/PyzVJMcoctSS7zGk3
u5miRTr/VSSslo9Oe2DN5yDKcE+qjQHKmE1IHGROUrKLJbv9KSY6GcEx7L35J/qP
9hX8EZJTPM8WXcgt06BaCk+YGITAGrdcZ9nZclJtVcfws2ddBDuP+zqDbsXCdd2c
Yw9vtkfiauoH0X04Sw3F72HqHtuLGYLvLrQBtqsIxWogtt1qBuC/g2OhG6bEIGpX
bpg8+rFq9IZlT0v2BQmFPB6Cx0sBG9rMt8zn/P0LXt9NaDOwy0IVy9fOM/38Bw==
-----END CERTIFICATE-----
 
)KEY";
 
 
// Copy contents from  XXXXXXXX-private.pem.key here ▼
static const char privkey[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEowIBAAKCAQEA5XOUfoesTOY8s5pNe3X68QZfGoSd9CfEvj/VPzGrm6NOOEXv
hIxKcCTHw1p+41WglfJue64wuRrmddTNbxkcAecZ5S5cA9VbStIc52HNCEPfYDnx
qRUws8wSmfbY545iKJ+E90izmqgM+Hn7JCTenKrMQBO7O+OW6C+HBprW+dnn2MXy
wlzNCkBkmUj9jXJrBBoRI9dqwL4JiB0sx+uFbdO8/z+vqqArNih/I+onyxC4mLTO
eGzjpbB9QOZGjz/Q/4/SZ8XYd8qbAUtenvKQIBZ8Y4CLzrG9edYXd/i4ba/Yb3Sb
4TYmxuh7UJNuiW+VauTvssQYbYTHhha929xGCwIDAQABAoIBAQDMLVOAXJ6YN3Dg
qXg1WsEY5YCPil88On86hHbwKvT4JBOgw6rlmilLLY+NaOPsy++APyDNci3tKeDW
Da1ZJVDk6I8Ef4Jl7t+z7tDFnSPkLPGDIUzIFxEljX0oAJ73Pp5eY8Tgf5NDFjOk
kHdIrH5jjyD+DBDatCU/K7vNxs9xuCTUsl7dS6XE23Fcx51qBFm0e6Dl5/T3bAaT
FJoC5QDvdjw0hr4lsR2mrnIvaQtHHgapIi2LdX6vs2SRA9O2I/fLH1+4JhGfijWQ
KGA7q2Q3IVXsMc2jz5qcNaTDKTSWRYqupCtUBbsgfzzxnvcyxKIub38e+IEJspLm
iqdwI6TxAoGBAPWyqAEVBodrEdU2AmEIMJJHyaysk/6qrtacjIT5JN5AlDN4Atzc
+/Rl/rzJGp71RqWQMl19xDbhPK8kAnqniy3e+qmlLoguddA/ouvmworQv+aQ4gle
L1Uupdz4cZC2Wjvk9vqpLl8PPek1NzGgnWEvkdCrI/5r7OzNlAoy9ReXAoGBAO8S
iJTIf5VYhQBp2OuLxckRxHEi/FTHilbpO7xhU6/XC+FhghTiYbrvnFYEViNVzjpe
biqmB3mIt3VZzweofrbQloQuwTyLDg9XBedWM6frrQXpe2j2lW48+2y0TSaBwvtQ
eO7R0LyaBne6yWgkAQ5IQhvMAXuz/Ar3u7uywfOtAoGAMkcb8BTAYEe9v3as5/RI
FDcSgv5RzqRY4NvqOW/OZpiqexnM5Ctod6vDhBhXFT419b1bQRxyYZSX2XMx+q18
Yqelfb+qlP2E6FeNXF69YrCzBFiTmL0oTHMeR+ijPKzELEt/au3h6lvxHF7ia4pl
ijS0zxGmbPLi6dZru+aPlwMCgYAJYPXAuHjg1/8nu/9Jhks1DyBgu9mgTc+shyvT
wRGyZUJPDi83OIZS+hFtAnCKpFQZ0BiMTcUfTHC+Bs7rYv3NmDM/bkIgaOgdS/Gs
WKJgJQu9nqI7xOip0XSfmBcquo52o3qLC+ofa/9y9P3vuvHbqJQHWSJ5wt7RY1SX
EXUwZQKBgCiatrpKlIF9RGcQ4yAUhJWihkODe4TDKAG66maK0sh42wt+WZeIWsk3
xsXE0qrVoXj3qAGD6OQwtU9pHMRfgfiAhOMSDIxo9wAeiSDb936A0nffiLZxXTYA
5NMmhHdzzt32p3kZreuUeU3Me9zbYKA8cDSpS5lPAtnGV6XwvdGI
-----END RSA PRIVATE KEY-----
 
)KEY";
