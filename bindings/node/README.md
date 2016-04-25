# LibKeen Node.js plugin

Hi! This is the repository of Helios' Keen IO plugin. This is a write-only plugin written for Node.js with caching support (no Keen administration/read operations. solely one-way metric writing to Keen servers).

Caching feature can come in handy if you are using Keen IO in situations where Internet connectivity is sparse. This library caches your calls and sends them at a later time when connection is back online and stable.

## Runtime Requirements

Node.js bindings to *libkeen* are "shallow" bindings to the ABI stable shared library of *libkeen*. The actual *libkeen* shared library is loaded during runtime with platform specific calls. For example on Windows, after you perform `npm install libkeen`, you need to place `libkeen.dll` in your current directory (usually next to where the executor script is located).

If you are getting module not found errors even after you place `libkeen.dll` in your current working directory, you might need to install [Visual C++ Redistributable for Visual Studio 2015](https://www.microsoft.com/en-us/download/details.aspx?id=48145).

## Sample usage

You may begin by:
```CMD
npm install libkeen
```

Then `require` it in your script:
```JAVASCRIPT
var libkeen = require("libkeen");
```

and start sending events:
```JAVASCRIPT
var client = new libkeen.client();

client.setProjectId("js id");
client.setWriteKey("js key");

var collection_name = "sample";
var event_json_data = JSON.stringify({ "action" : "data" });

client.sendEvent(collection_name, event_json_data);
```

At exit, plugin automatically handles pending `sendEvent` calls.
