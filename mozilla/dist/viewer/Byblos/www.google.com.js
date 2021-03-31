/* General cleanup of Google services. 
   Cameron Kaiser */

function() {
	return {
		_myURI : null,
		_mode : '',
		
		// Utility function to extract pieces out of a URI path.
		_getarg : function(name, upath) {
			name = name.replace(/[\[]/,"\\\[").replace(/[\]]/,"\\\]");
			var nx = "[\\?&]"+name+"=([^&#]*)";
			var rx = new RegExp(nx);
  			var results = rx.exec(upath);
  			return (results == null) ? "" : decodeURI(results[1]);
		},
		
		// Same thing, but dequotes it to prevent a security hole.
		_safegetarg : function(name, upath) {
			var x = this._getarg(name, upath);
			if (!x.length) return x;
			
			x = x.replace(/"/, '');
			return x;
		},
		
		wantURI : function(uri) {
			this._myURI = uri;
			this._mode = '';
			
			/* Clean up image results. */
			if (uri.path.match(/^\/imgres\?/)) {
				this._mode = "imgr";
				return true;
			}
			
			return false;
		},
		
		parseHTML : function(text) {
			var answer = text;

			/* This takes Google Images result URLs and makes a simple page for them. */
			if (this._mode == 'imgr') {
				var imgurl = this._safegetarg('imgurl', this._myURI.path);
				var imgrefurl = this._safegetarg('imgrefurl', this._myURI.path);
				answer = "<html>"+
				"<body><center>"+
				"<h3>Byblos generated result for Google Image Search</h3>"+
				'URL: <a href="' + imgrefurl + '">'+ imgrefurl + '</a><p>' +
				'<img src="' + imgurl + '"><p>' +
				"</center></body>"+
				"</html>";
			}
						
			return {
				response : "ok",
				body     : answer
			};
		}
	};
};