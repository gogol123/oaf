using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;

using System.Net;
using System.Threading;
using Kayak;
using Kayak.Http;

namespace OAFKayak
{
    class SchedulerDelegate : ISchedulerDelegate
    {
        public void OnException(IScheduler scheduler, Exception e)
        {

            e.DebugStackTrace();
        }

        public void OnStop(IScheduler scheduler)
        {

        }
    }

    class RequestDelegate : IHttpRequestDelegate
    {
        public void OnRequest(HttpRequestHead request, IDataProducer requestBody,
            IHttpResponseDelegate response)
        {
            string body = "error";
            if (request.Uri.StartsWith("/ntm"))
            {

                if (request.Uri.EndsWith("kill"))
                {
                    Process[] processes = Process.GetProcessesByName("AsTelOS Express");
                    foreach (Process process in processes)
                    {
                        process.Kill();
                        
                    }
                    processes = Process.GetProcessesByName("ttTCLM_ASCOM_Telescope");
                    foreach (Process process in processes)
                    {
                        process.Kill();
                        
                    }
                    body = "{ok}";

                }
              
                var headers = new HttpResponseHead()
                {
                    Status = "200 OK",
                    Headers = new Dictionary<string, string>() 
                    {
                        { "Content-Type", "text/plain" },
                        { "Content-Length", body.Length.ToString() },
                    }
                };
                response.OnResponse(headers, new BufferedProducer(body));
            }
            else
            {
                var responseBody = "The resource you requested ('" + request.Uri + "') could not be found.";
                var headers = new HttpResponseHead()
                {
                    Status = "404 Not Found",
                    Headers = new Dictionary<string, string>()
                    {
                        { "Content-Type", "text/plain" },
                        { "Content-Length", responseBody.Length.ToString() }
                    }
                };
                var bodyerr = new BufferedProducer(responseBody);

                response.OnResponse(headers, bodyerr);
            }
        }
    }

    class BufferedProducer : IDataProducer
    {
        ArraySegment<byte> data;

        public BufferedProducer(string data) : this(data, Encoding.UTF8) { }
        public BufferedProducer(string data, Encoding encoding) : this(encoding.GetBytes(data)) { }
        public BufferedProducer(byte[] data) : this(new ArraySegment<byte>(data)) { }
        public BufferedProducer(ArraySegment<byte> data)
        {
            this.data = data;
        }

        public IDisposable Connect(IDataConsumer channel)
        {
            // null continuation, consumer must swallow the data immediately.
            channel.OnData(data, null);
            channel.OnEnd();
            return null;
        }
    }

    class BufferedConsumer : IDataConsumer
    {
        List<ArraySegment<byte>> buffer = new List<ArraySegment<byte>>();
        Action<string> resultCallback;
        Action<Exception> errorCallback;

        public BufferedConsumer(Action<string> resultCallback,
    Action<Exception> errorCallback)
        {
            this.resultCallback = resultCallback;
            this.errorCallback = errorCallback;
        }
        public bool OnData(ArraySegment<byte> data, Action continuation)
        {
            // since we're just buffering, ignore the continuation. 
            // TODO: place an upper limit on the size of the buffer. 
            // don't want a client to take up all the RAM on our server! 
            buffer.Add(data);
            return false;
        }
        public void OnError(Exception error)
        {
            errorCallback(error);
        }

        public void OnEnd()
        {
            // turn the buffer into a string. 
            // 
            // (if this isn't what you want, you could skip 
            // this step and make the result callback accept 
            // List<ArraySegment<byte>> or whatever) 
            // 
            var str = buffer
                .Select(b => Encoding.UTF8.GetString(b.Array, b.Offset, b.Count))
                .Aggregate((result, next) => result + next);

            resultCallback(str);
        }
    } 
}
