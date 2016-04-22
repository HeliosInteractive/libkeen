namespace Helios.Keen
{
    using System;
    using UnityEngine;
    using System.Reflection;
    using System.Collections;
    using System.Runtime.InteropServices;

    public partial class Client : MonoBehaviour
    {
        Bindings        m_Bindings      = null;
        static bool     s_HandledQuit   = false;
        float           m_CacheInterval = 10;
        int             m_CacheCount    = 10;

        /// <summary>
        /// Project ID, obtained from Keen IO's dashboard
        /// </summary>
        public string ProjectId
        {
            get { return m_Bindings.ProjectId; }
            set { m_Bindings.ProjectId = value; }
        }

        /// <summary>
        /// Write Key, obtained from Keen IO's dashboard
        /// </summary>
        public string WriteKey
        {
            get { return m_Bindings.WriteKey; }
            set { m_Bindings.WriteKey = value; }
        }

        /// <summary>
        /// The amount in seconds to wait in-between attempts
        /// to post cached events.
        /// </summary>
        public float CacheInterval
        {
            get { return m_CacheInterval; }
            set { m_CacheInterval = value; }
        }

        /// <summary>
        /// The maximum number of cached events to be sent
        /// at every CacheInterval.
        /// </summary>
        public int CacheCount
        {
            get { return m_CacheCount; }
            set { m_CacheCount = value; }
        }

        /// <summary>
        /// Sends a raw event to Keen IO
        /// </summary>
        /// <param name="collection">Name of the Keen IO collection</param>
        /// <param name="json">JSON data of the event</param>
        public virtual void SendEvent(string collection, string json)
        {
            m_Bindings.SendEvent(collection, json);
        }

        /// <summary>
        /// Sends an event to Keen IO
        /// </summary>
        /// <typeparam name="T">C# type of the event</typeparam>
        /// <param name="collection">Name of the Keen IO collection</param>
        /// <param name="event_data">C# object containing event data</param>
        public void SendEvent<T>(string collection, T event_data)
        {
            SendEvent(collection, Serialize(event_data));
        }

        void Awake()
        {
            // Allocate native instance
            m_Bindings = new Bindings();
        }

        void Start()
        {
            // Kick start cache routing
            StartCoroutine(CacheRoutine());
        }

        void OnDestroy()
        {
            // Stop cache routine
            StopAllCoroutines();
        }

        IEnumerator CacheRoutine()
        {
            // Post a cache sweep jon to thread pool
            Bindings.PostCache(CacheCount);
            yield return new WaitForSeconds(CacheInterval);
        }

        public void OnApplicationQuit()
        {
            if (!s_HandledQuit)
            {
                // Wait for pending handlers
                Bindings.Flush();
                Bindings.Release();
                s_HandledQuit = true;
            }
        }

        #region Basic JSON serializer
        /// <summary>
        /// A VERY limited JSON serialize just for convenience
        /// IF YOU NEED MORE ADVANCED SERIALIZATION, use SendEvent(string,string)
        /// overload with your own JSON library. This method can only serialize
        /// flat struct and classes with no polymorphisms.
        /// </summary>
        /// <typeparam name="T">type to be serialized</typeparam>
        /// <param name="obj">value of the object</param>
        /// <returns></returns>
        public string Serialize<T>(T obj)
        {
            // Take in objects of type struct or class only.
            if (obj == null || !(typeof(T).IsValueType || typeof(T).IsClass))
                return string.Empty;

            string json = "{";

            foreach (FieldInfo info in typeof(T).GetFields(BindingFlags.Public | BindingFlags.Instance))
            {
                string key = string.Format("\"{0:s}\"", info.Name);
                string val = "\"error\"";

                // This covers all integral types (type double in json)
                if (info.FieldType.IsPrimitive)
                {
                    if (info.FieldType == typeof(bool))
                    {
                        val = info.GetValue(obj).ToString().ToLower();
                    }
                    else
                    {
                        val = string.Format("{0:g}", info.GetValue(obj));
                    }
                }
                // this covers Enums and casts it to an integer
                else if (info.FieldType.IsEnum)
                {
                    val = string.Format("{0:g}", (int)info.GetValue(obj));
                }
                // This handles classes and struct and recurses into Serialize again
                else if (info.FieldType != typeof(string) &&
                    (info.FieldType.IsClass || info.FieldType.IsValueType))
                {
                    // This invokes our Generic method with
                    // a dynamic type info during runtime.
                    val = GetType()
                        .GetMethod(MethodInfo.GetCurrentMethod().Name)
                        .MakeGenericMethod(info.FieldType)
                        .Invoke(this, new object[] { info.GetValue(obj) })
                        .ToString();
                }
                // Handle everything else as a string
                else
                {
                    object runtime_val = info.GetValue(obj);

                    if (runtime_val == null)
                        val = "null";
                    else
                        val = string.Format("\"{0}\"", runtime_val.ToString().Replace("\"", "\\\""));
                }

                string key_val = string.Format("{0}:{1},", key, val);
                json += key_val;
            }

            json = json.TrimEnd(',');
            json += "}";

            return json;
        }
        #endregion

        private class Bindings : IDisposable
        {
            IntPtr  m_NativeInstance    = IntPtr.Zero;
            bool    m_Disposed          = false;
            string  m_ProjectId         = "invalid";
            string  m_WriteKey          = "invalid";

            public Bindings()
            {
                m_NativeInstance = libkeen_client_keenio_make();
            }

            public string ProjectId
            {
                get { return m_ProjectId; }
                set
                {
                    m_ProjectId = value;
                    libkeen_client_keenio_set_project_id(m_NativeInstance, m_ProjectId);
                }
            }

            public string WriteKey
            {
                get { return m_WriteKey; }
                set
                {
                    m_WriteKey = value;
                    libkeen_client_keenio_set_write_key(m_NativeInstance, m_WriteKey);
                }
            }

            public void SendEvent(string collection, string json)
            {
                libkeen_client_send_event(m_NativeInstance, collection, json);
            }

            static public void Flush()
            {
                libkeen_core_flush();
            }

            static public void Release()
            {
                libkeen_core_release();
            }

            static public void PostCache(int count)
            {
                libkeen_core_post_cache(count);
            }

            #region Native API
            [DllImport("libkeen")]
            public static extern int libkeen_version_major();

            [DllImport("libkeen")]
            public static extern int libkeen_version_minor();

            [DllImport("libkeen")]
            public static extern int libkeen_version_patch();

            [DllImport("libkeen")]
            public static extern IntPtr libkeen_client_keenio_make();

            [DllImport("libkeen")]
            public static extern void libkeen_client_keenio_free(IntPtr ptr);

            [DllImport("libkeen")]
            public static extern void libkeen_client_keenio_set_project_id(IntPtr ptr, string val);

            [DllImport("libkeen")]
            public static extern void libkeen_client_keenio_set_write_key(IntPtr ptr, string val);

            [DllImport("libkeen")]
            public static extern void libkeen_client_send_event(IntPtr ptr, string col, string json);

            [DllImport("libkeen")]
            public static extern void libkeen_core_flush();

            [DllImport("libkeen")]
            public static extern void libkeen_core_release();

            [DllImport("libkeen")]
            public static extern void libkeen_core_post_cache(int count);
            #endregion

            #region IDisposable Support
            protected virtual void Dispose(bool disposing)
            {
                if (!m_Disposed)
                {
                    if (disposing)
                    {
                        // dispose managed state (managed objects).
                    }

                    libkeen_client_keenio_free(m_NativeInstance);
                    m_NativeInstance = IntPtr.Zero;
                    m_Disposed = true;
                }
            }

            ~Bindings()
            {
                Dispose(false);
            }

            public void Dispose()
            {
                Dispose(true);
                GC.SuppressFinalize(this);
            }
            #endregion
        }
    }
}
