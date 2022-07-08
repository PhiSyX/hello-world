/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

use mikuscript_playground::Counter;

import @css "assets/mscss/jit.css" {
	multiple_of: 8;
};

import @html "index.html" {
	head {
		title = "Mon super titre!";

		style = @css "assets/mscss/jit.css";
	}

	body { // @slot
		<h2>Cette balise est générée depuis le script "./src/main.ms".</h2>

		<Counter />
	}
};

async fn main() {
	let server_config = mikuscript::ServerConfig::try_from("config/site.json")
		.expect("Le fichier de configuration du serveur.");

	let routes = mikuscript::routes!(
		[ GET ] "/" => (@html "index.html").render()
	);

	#[launch]
	mikuscript::server! server_config {
		mikuscript::serve(routes)
	};
}
