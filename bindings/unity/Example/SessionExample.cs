using System.IO;
using UnityEngine;

[RequireComponent(typeof(Helios.Keen.SessionAwareClient))]
public class SessionExample : MonoBehaviour
{
    public Helios.Keen.SessionAwareClient MetricsClient;

    void Awake()
    {
        // Add a metrics client as a member
        if (MetricsClient == null)
            MetricsClient = gameObject.AddComponent<Helios.Keen.SessionAwareClient>();
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

        // Start a new session with a random UUID
        MetricsClient.StartSession();

        // This is an example of sending Helios specific events
        MetricsClient.SendQuizEvent(new Helios.Keen.Client.QuizEvent
        {
            quizId      = "IQ test",
            quizResult  = "failed",
            experienceData  = new Helios.Keen.Client.ExperienceData
            {
                experienceLabel = "Keen Plugin",
                versionNumber   = "1.0.0",
                location        = "never land"
            }
        });

        // End current session
        MetricsClient.EndSession(Helios.Keen.Client.EndSessionType.Completed);

        // Start a new session with a known guestId
        MetricsClient.StartSession(new Helios.Keen.Client.Session { guestId = "myGuestId" });

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

        // End current session
        MetricsClient.EndSession(Helios.Keen.Client.EndSessionType.Abandoned);
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
