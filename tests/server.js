var http = require('http');
var sc = false;
var count = 0;

http.createServer(function(request, response)
{
  if (request.url == '/shutdown') {
	process.exit(0);
  } else if (sc || request.url == '/pass') {
	response.writeHead(200, {"Content-Type": "text/plain"});
	response.end("OK");
	count++;
  } else if (request.url == '/short_circuit') {
	response.writeHead(200, {"Content-Type": "text/plain"});
	response.end("OK");
	sc = true;
  }  else if (request.url == '/count') {
	response.writeHead(200, {"Content-Type": "text/plain"});
	response.end("" + count);
  } else if (request.url == '/clear') {
	response.writeHead(200, {"Content-Type": "text/plain"});
	response.end("CLEAR");
	count = 0;
  } else if (request.url == '/fail') {
	response.writeHead(400);
	response.end("FAIL");
  } else {
	response.writeHead(403);
	response.end("NOPE");
  }
}).listen(8080);
