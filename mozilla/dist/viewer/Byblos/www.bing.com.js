/* Stele for bing front page
   Cameron Kaiser
   Public domain */

function() {
	return {
		wantURI : function(uri) {
			if (uri.path == "/" || uri.path == "")
				return true;
			return false;
		},
		parseHTML : function(text) {
			var page;
			
			return {
				response : "ok",
				body     : ''+
				
				'<html>'+
				'<head><title>Bing (converted by Byblos)</title></head>'+
				'<body>'+
				'<form action="/search" method="get">'+
				'Search Bing:'+
				'<input type="text" name="q">'+
				'<input type="submit" value="Search">'+
				'</form>'+
				'<p>'+
				'&copy;Microsoft'+
				'<p>'+
				'<a href="http://go.microsoft.com/fwlink/?LinkId=521839">Privacy</a> '+
				'<a href="http://go.microsoft.com/fwlink/?LinkID=246338">Legal</a>'
			};
		}
	};
};
