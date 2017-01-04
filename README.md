# tr181cli

CLI for for WebPA interactions with TR-181 Objects

[![Build Status](https://travis-ci.org/Comcast/tr181cli.svg?branch=master)](https://travis-ci.org/Comcast/tr181cli)
[![codecov.io](http://codecov.io/github/Comcast/tr181cli/coverage.svg?branch=master)](http://codecov.io/github/Comcast/tr181cli?branch=master)
[![Apache V2 License](http://img.shields.io/badge/license-Apache%20V2-blue.svg)](https://github.com/Comcast/tr181cli/blob/master/LICENSE)

# Building and Testing Instructions

```
mkdir build
cd build
cmake ..
make
make test
```

# Usage Instructions

```
Currently tr181cli is supporting GET queries.

Sample command line GET query is as below:
./tr181cli Get parameter1 parameter2
e.g. --> ./tr181cli Get Device.DeviceInfo.ModelName Device.DeviceInfo.Manufacturer

tr181cli works as:

-tr181cli will accept the command line arguments with command and required parameter names
-converts this request into wrp-c format 
-wrp-c formatted request will be encoded using msgpack
-sends this encoded request to webpa adapter through nanomsg
-webpa adapter will process this requests to fetch the values from stack
-tr181cli accepts the encoded response from adapter through nanomsg and displays the TR181 query result.
```

