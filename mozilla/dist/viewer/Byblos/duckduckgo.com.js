/* Stele for DuckDuckGo front page: force /html/ version
   Cameron Kaiser
   Public domain */

function() {
	return {
		wantURI : function(uri) {
			if (uri.path == "/" || uri.path == "")
				return true;
			return false;
		},
		fullReplace : function(uri) {
			if (uri.path == "/" || uri.path == "")
				return true;
			return false;
		},
		parseHTML : function(text) {
			
			return {
				response : "ok",
				body     : 
				
				'<meta http-equiv="refresh" content="0; url=http://lite.duckduckgo.com/lite/">'	
			};
		}
	};
};
