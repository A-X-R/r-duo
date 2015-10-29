import cherrypy

try:
    import serial
    ser = serial.Serial('/dev/ttyAMA0', 115200)
except:
    ser = None
    print "Error starting serial"

try:
    import RPi.GPIO as GPIO
    GPIO.setwarnings(False)
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(18, GPIO.OUT)
    GPIO.output(18, True)
except:
    print "Error setting GPIO output"


allowed = {'a', 's', 'd'}

class RduoRemote:
    exposed = True

    def GET(self):
        return

    def PUT(self, direction=None, whistle=None):

        if direction is not None and direction in allowed:
            ret = direction
        elif whistle is not None:
            ret = 'w'
        else:
            ret = "error"

        if ser is not None and ret is not "error":
            ser.write(ret)
        elif ser is None:
            print ret
        return ret

    def OPTIONS(self):
        return

def CORS():
    cherrypy.response.headers["Access-Control-Allow-Origin"] = "*"
    cherrypy.response.headers["Access-Control-Allow-Methods"] = "POST, GET, OPTIONS, PUT"

cherrypy.tools.CORS = cherrypy.Tool('before_handler', CORS)

if __name__ == '__main__':

    cherrypy.tree.mount(
        RduoRemote(), '/', {
            '/': {
                'request.dispatch': cherrypy.dispatch.MethodDispatcher()
            }
        }
    )

    cherrypy.response.headers["Access-Control-Allow-Origin"] = "*"

    cherrypy.config.update(
        {
            'server.socket_host': '0.0.0.0',
            'server.socket_port': 80,
            'server.thread_pool': 1,
        }
    )

    conf = {
        '/': {
            'tools.CORS.on': True,
            'request.dispatch': cherrypy.dispatch.MethodDispatcher(),
        }
    }

    cherrypy.quickstart(RduoRemote(), '/', conf)
