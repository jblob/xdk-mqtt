#!/usr/bin/env python

import sys
from pprint import pprint

if __name__ == '__main__':
    print(sys.argv[:-1])
    data = {'server' : '{}'.format(sys.argv[3])}
    with open(sys.argv[1]) as data_file:
        user = "user"
        password = "password"
        clientId = "clientId"
        topic = "topic"
        data = dict(data, **eval(data_file.read()))

    with open(sys.argv[2], 'w') as header:
        header.write('#pragma once\n\r\n\r')
        for key, val in data.items():
            headerLine = '#define MQTT_{} "{}"\n\r'.format(key.upper(), val)
            header.write(headerLine)
        header.write('#define MQTT_SERVER_PORT {}\n\r'.format(sys.argv[4]))
        header.write('#define WLAN_CONNECT_WPA_SSID "{}"\n\r'.format(sys.argv[5]))
        header.write('#define WLAN_CONNECT_WPA_PASS "{}"\n\r'.format(sys.argv[6]))
        header.write('\n\r')
