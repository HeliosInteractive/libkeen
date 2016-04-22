# LibKeen Unity3D plugin

Hi! This is the repository of Helios' Keen IO plugin. This is a write-only plugin written specifically for Unity3D with caching support (no Keen administration/read operations. solely one-way metric writing to Keen servers).

Caching feature can come in handy if you are using Keen IO in situations where Internet connectivity is sparse. This library caches your calls and sends them at a later time when connection is back online and stable.

## Sample usage

You may begin by importing the latest stable release `.unitypackage`. There is an example script (`MetricsExample.cs`) in this repository which shows you how to use this plugin.

----

Start by having an instance of `Helios.Keen.Client` which is a `MonoBehaviour`:

```C#
var MetricsClient = gameObject.AddComponent<Helios.Keen.Client>();
```

Then provide it your project's specific settings:

```C#
MetricsClient.ProjectId = "project id";
MetricsClient.WriteKey = "write key";
```

And start sending events:

```C#
// This is an example of sending Helios specific events
MetricsClient.SendQuizEvent(new Helios.Keen.Client.QuizEvent
{
	quizId = "IQ test",
	quizResult = "failed",
	experienceData  = new Helios.Keen.Client.ExperienceData
	{
		experienceLabel = "Keen Plugin",
		versionNumber   = "1.0.0",
		location        = "never land"
	}
});

// This is an example of using custom data types
MetricsClient.SendEvent("custom_event", new CustomData
{
	data_member_1 = "test string",
	data_member_2 = 25000.0f,
	data_member_3 = new CustomNestedData
	{
		data_member_1 = "\"nested\" string",
		data_member_2 = 25000d,
	}
});
```

Don't forget to cleanup after yourself! (In case you used `AddComponent`)

```C#
Destroy(MetricsClient);
```

Take a look at `MetricsExample.cs` for more in-depth usage examples.
Also `SessionExample.cs` shows you how to use `StateAwareClient` class.

## Built-in JSON serializer

There is an extremely simplistic JSON serializer built into this library (about 60 lines of code with comments!) which provides a portable and backwards compatible C# implementation for serializing **FLAT CLASSES, FLAT STRUCTS, and POD data types**.

This means the serializer *does not support fancy features such as inheritance*. You can absolutely use a custom and more advanced JSON serializer; if you absolutely need to work with more complicated data types. Here's an example of using [Unity 5.3's JSON serializer](http://docs.unity3d.com/Manual/JSONSerialization.html) and using the `SendEvent(string, string)` overload:

```C#
MetricsClient.SendSession("eventName", JsonUtility.ToJson(myComplexObject));
```

## Helios extensions

Helios internally uses some conventions when dealing with Keen.IO. These conventions can be found in `Helios/Keen/ClientHeliosExtension.cs`.
