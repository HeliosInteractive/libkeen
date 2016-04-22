using System.IO;
using UnityEngine;

[RequireComponent(typeof(Helios.Keen.Client))]
public class MetricsExample : MonoBehaviour
{
    public Helios.Keen.Client MetricsClient;

    void Awake()
    {
        // Add a metrics client as a member
        if (MetricsClient == null)
            MetricsClient = gameObject.AddComponent<Helios.Keen.Client>();
    }

    void OnDestroy()
    {
        // Remember to clean up after yourself!
        if (MetricsClient != null)
            Destroy(MetricsClient);
    }

    void Start()
    {
        // These lines assign project settings
        MetricsClient.ProjectId = "none";
        MetricsClient.WriteKey = "none";
    }

    void Update ()
    {
        // Let's send a bunch of metric events on mouse click...

        if (!Input.GetMouseButtonDown(0))
            return;

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
    }

    class CustomData
    {
        public string           data_member_1;
        public float            data_member_2;
        public CustomNestedData data_member_3;
    }

    class CustomNestedData
    {
        public string   data_member_1;
        public double   data_member_2;
    }
}
