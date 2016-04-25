{
	"targets": [
	{
		"target_name": "libkeen",
		"sources": [
			"libkeen.cc",
			"version.hh",
			"version.cc",
			"client.hh",
			"client.cc",
			"core.hh",
			"core.cc",
			"dl.hh",
			"dl.cc"
		],
		"include_dirs": [
			"<!(node -e \"require('nan')\")"
		]
	}]
}
