#!/usr/bin/env python

# with help from https://github.com/msaunby/ble-sensor-pi/blob/master/sensortag/sensortag.py
# Michael Saunby. April 2013

from ctypes.util import find_library
import pexpect, time

import matplotlib
matplotlib.use('GTKAgg')
import matplotlib.pyplot as plt

#if not os.geteuid() == 0:
#    sys.exit("script only works as root")

btlib = find_library("bluetooth")
if not btlib:
    raise Exception(
        "Can't find required bluetooth libraries"
    )
    
class bleBot:

    def __init__( self, ble_adr ):
        self.ble_adr = ble_adr
        self.con = pexpect.spawn('gatttool -b ' + self.ble_adr + ' -I -t random')
        self.con.delaybeforesend = 0 #THIS LINE IS SUPER IMPORTANT
        self.con.expect('\[LE\]', timeout=1)
        self.handle = 'b' #!! this is the TX service on the nRF8001 adafruit breakout with callbackEcho sketch

    def connect( self ):
        # OH HEXAPOD this is so sketchy, it will break if bluez gatttool changes at all. I have version 5.20
        print "Preparing to connect. Address: " + self.ble_adr
        self.con.sendline('connect')
        try:
            self.con.read_nonblocking(2048,0) #flush the read pipe!! SUPER IMPORTANT
        except:
            pass
#        i = self.con.expect(['Attempting', 'Error'], timeout=1)
        i = 0
        if i == 0:
            #print 'Attempting to connect'
#            j = self.con.expect(['Connection successful', 'No route', 'busy', pexpect.TIMEOUT], timeout = 1)
            j = 0            
            if j == 0:
                print self.ble_adr, ': connected!'
            if j == 1:
                print self.ble_adr, ': No route to host, is USB dongle plugged in?'
                self.cleanup()
            if j == 2:
                print self.ble_adr, ': Device busy, is something else already connected to it?'
                c = True
                while c:
                    inp = raw_input('Try hitting reset. Type "y" to continue or "n" to quit.')
                    if inp.lower().startswith('y'):
                        self.con.sendline('connect')
                        try:
                            self.con.read_nonblocking(2048,0) #flush the read pipe!! SUPER IMPORTANT
                        except:
                            pass
                        k = self.con.expect(['Connection successful', pexpect.TIMEOUT], timeout = 1)
                        print 'k: ', k
                        if k == 0:
                            print self.ble_adr, ': connected!'
                        if k == 1:
                            print self.ble_adr, ': Could not connect'
                            self.cleanup()
                        break
                    elif inp.lower().startswith('n'):
                        self.cleanup()
                        break
                    else:
                        print 'Did not understand command. Try again.'
            if j == 3:
                print 'Attempting to connect, is device on and in range? '
                #foostr = raw_input('Type anything to continue, or enter to cancel')
                self.con.sendline('connect')
                try:
                    self.con.read_nonblocking(2048,0) #flush the read pipe!! SUPER IMPORTANT
                except:
                    pass
                k = self.con.expect(['Connection successful', pexpect.TIMEOUT], timeout = 3)
                print 'k: ', k
                if k == 0:
                    print self.ble_adr, ': connected!'
                if k == 1:
                    print self.ble_adr, ': Could not connect'
                    self.cleanup()
        if i == 1:
            print 'Is USB dongle plugged in?'
            self.cleanup()
        return self
         
    def char_write_cmd( self, handle, value):
        cmd = 'char-write-cmd 0x%s %s' % (handle, value)
        #print self.ble_adr, cmd
        self.con.sendline(cmd)
        s = ''
        try:
            s = self.con.read_nonblocking(2048,0) #flush the read pipe!! SUPER IMPORTANT
        except:
            pass
        #print 'After sending command, before: ', self.con.before, 'after :', self.con.after
        return s

    def char_write_req(self,handle,value):
        cmd = 'char-write-cmd 0x%s %s' % (handle, value)
        #print self.ble_adr, cmd
        self.con.sendline(cmd)
        try:
            print self.con.read_nonblocking(2048,0) #flush the read pipe!! SUPER IMPORTANT
        except:
            pass
        #print 'After sending command, before: ', self.con.before, 'after :', self.con.after
        return

    def cleanup( self ):
        print self.ble_adr, ': attempting to disconnect'
        try:
            self.con.sendline('disconnect')
            self.con.sendline('exit')
            try:
                self.con.read_nonblocking(2048,0) #flush the read pipe!! SUPER IMPORTANT
            except:
                pass
            isalive = self.con.terminate(force=True)
            print self.ble_adr, ': has been terminated? ', isalive
            self.con.close(force=True)
            #print self.ble_adr, 'is alive: ', self.con.isalive()
        except OSError:
            print self.ble_adr, ': OSError'
            pass
        return



def tupToHex( foolist ):
    hexed = ''
    for i in foolist :
        hexed += format(i, '02x')
    #print hexed
    return hexed

#def worker( address, commands ):
def worker( cmdQueue, connection):
    while True:
        cmd = cmdQueue.get()
        #print 'queue items left: ', cmdQueue.qsize()
        print connection.ble_adr, 'rcvd from queue:', cmd
        #print connection.ble_adr, ': queue size: ', cmdQueue.qsize()
        if cmd is None:
            print connection.ble_adr, ': attempting to cleanup'
            connection.cleanup()
            return 
        else:
            connection.char_write_cmd(tupToHex(cmd))

import numpy as np

class Quaternion:
    """Quaternions for 3D rotations"""
    def __init__(self, x):
        self.x = np.asarray(x, dtype=float)
        
    @classmethod
    def from_v_theta(cls, v, theta):
        """
        Construct quaternion from unit vector v and rotation angle theta
        """
        theta = np.asarray(theta)
        v = np.asarray(v)
        
        s = np.sin(0.5 * theta)
        c = np.cos(0.5 * theta)
        vnrm = np.sqrt(np.sum(v * v))

        q = np.concatenate([[c], s * v / vnrm])
        return cls(q)

    def __repr__(self):
        return "Quaternion:\n" + self.x.__repr__()

    def __mul__(self, other):
        # multiplication of two quaternions.
        prod = self.x[:, None] * other.x

        return self.__class__([(prod[0, 0] - prod[1, 1]
                                 - prod[2, 2] - prod[3, 3]),
                                (prod[0, 1] + prod[1, 0]
                                 + prod[2, 3] - prod[3, 2]),
                                (prod[0, 2] - prod[1, 3]
                                 + prod[2, 0] + prod[3, 1]),
                                (prod[0, 3] + prod[1, 2]
                                 - prod[2, 1] + prod[3, 0])])
    
    def inv(self):
        return self.__class__([self.x[0],-self.x[1],-self.x[2],-self.x[3]])

    def as_v_theta(self):
        """Return the v, theta equivalent of the (normalized) quaternion"""
        # compute theta
        norm = np.sqrt((self.x ** 2).sum(0))
        theta = 2 * np.arccos(self.x[0] / norm)

        # compute the unit vector
        v = np.array(self.x[1:], order='F', copy=True)
        v /= np.sqrt(np.sum(v ** 2, 0))

        return v, theta

    def as_rotation_matrix(self):
        """Return the rotation matrix of the (normalized) quaternion"""
        v, theta = self.as_v_theta()
        c = np.cos(theta)
        s = np.sin(theta)

        return np.array([[v[0] * v[0] * (1. - c) + c,
                          v[0] * v[1] * (1. - c) - v[2] * s,
                          v[0] * v[2] * (1. - c) + v[1] * s],
                         [v[1] * v[0] * (1. - c) + v[2] * s,
                          v[1] * v[1] * (1. - c) + c,
                          v[1] * v[2] * (1. - c) - v[0] * s],
                         [v[2] * v[0] * (1. - c) - v[1] * s,
                          v[2] * v[1] * (1. - c) + v[0] * s,
                          v[2] * v[2] * (1. - c) + c]])
                          
                          
class CubeAxes(plt.Axes):
    """An Axes for displaying a 3D cube"""
    # fiducial face is perpendicular to z at z=+1
    one_face = np.array([[1, 1, 1], [1, -1, 1], [-1, -1, 1], [-1, 1, 1], [1, 1, 1]])

    # construct six rotators for the face
    x, y, z = np.eye(3)
    rots = [Quaternion.from_v_theta(x, theta) for theta in (np.pi / 2, -np.pi / 2)]
    rots += [Quaternion.from_v_theta(y, theta) for theta in (np.pi / 2, -np.pi / 2)]
    rots += [Quaternion.from_v_theta(y, theta) for theta in (np.pi, 0)]
    
    # colors of the faces
    colors = ['blue', 'green', 'white', 'yellow', 'orange', 'red']
    
    def __init__(self, fig, rect=[0, 0, 1, 1], *args, **kwargs):
        # We want to set a few of the arguments
        kwargs.update(dict(xlim=(-2.5, 2.5), ylim=(-2.5, 2.5), frameon=False,
                           xticks=[], yticks=[], aspect='equal'))
        super(CubeAxes, self).__init__(fig, rect, *args, **kwargs)
        self.xaxis.set_major_formatter(plt.NullFormatter())
        self.yaxis.set_major_formatter(plt.NullFormatter())
        
        # define the current rotation
        self.current_rot = Quaternion([1,0,0,0])
        self.rot_cal = Quaternion([1,0,0,0]) # identity
        # define axes for Up/Down motion and Left/Right motion
        self._v_LR = (0, 1, 0)
        self._v_UD = (-1, 0, 0)
        self._active = False
        self._xy = None

        # connect some GUI events
        self.figure.canvas.mpl_connect('button_press_event',
                                       self._mouse_press)
        self.figure.canvas.mpl_connect('button_release_event',
                                       self._mouse_release)
        
    
    def draw_cube(self):
        """draw a cube rotated by theta around the given vector"""
        # rotate the six faces
        Rs = [(self.current_rot * self.rot_cal.inv() * rot).as_rotation_matrix() for rot in self.rots]
        faces = [np.dot(self.one_face, R.T) for R in Rs]
        
        # project the faces: we'll use the z coordinate
        # for the z-order
        faces_proj = [face[:, :2] for face in faces]
        zorder = [face[:4, 2].sum() for face in faces]
        
        # create the polygons if needed.
        # if they're already drawn, then update them
        if not hasattr(self, '_polys'):
            self._polys = [plt.Polygon(faces_proj[i], fc=self.colors[i],
                                       alpha=0.9, zorder=zorder[i])
                           for i in range(6)]
            for i in range(6):
                self.add_patch(self._polys[i])
        else:
            for i in range(6):
                self._polys[i].set_xy(faces_proj[i])
                self._polys[i].set_zorder(zorder[i])
                
        self.figure.canvas.draw()
        
    #----------------------------------------------------------
    # while the mouse button is pressed, set state to "active"
    # so that motion event will rotate the plot
    def _mouse_press(self, event):
        """Handler for mouse button press"""
        if event.button == 1:
            self._active = True
            self._xy = (event.x, event.y)

    def _mouse_release(self, event):
        """Handler for mouse button release"""
        if event.button == 1:
            self._active = False
            self._xy = None
            self.rot_cal = self.current_rot

def create_quaternion(data):
    q = [0,0,0,0]
    q[0] = int(data[4:8],16) / 16384.0;
    q[1] = int(data[8:12],16) / 16384.0;
    q[2] = int(data[12:16],16) / 16384.0;
    q[3] = int(data[16:20],16) / 16384.0;
    for i in range(len(q)): 
        if (q[i] >= 2): 
            q[i] = -4 + q[i]
#    return Quaternion([-q[0],-q[1],q[3],-q[2]]) # mirror about z-axis (like looking in a mirror)
    return Quaternion([q[0],-q[1],q[3],q[2]]) # no mirror, matches your motion

def main():
    plt.ion()
    
    fig = plt.figure(figsize=(4, 4))
    ax = CubeAxes(fig)
    fig.add_axes(ax)
    fig.canvas.show()
    
    ax.current_rot = Quaternion([1,0,0,0])
    ax.draw_cube()
    
    ble = bleBot("F2:14:C8:11:BB:5D")
    ble.connect()
    time.sleep(1)
    ble.char_write_req("e","0100")
    time.sleep(1)
    ble.char_write_cmd("b","4849")
    time.sleep(1)
    
    period = .1
    freq = 10.0
    period = 1/freq

    while(1):
        try:
            data = ble.char_write_cmd("b","4849").split("0x000d value: ")[1].split(" \r\n")[0].replace(" ","")
        except:
            pass # just use the data from last time because something screwed up...            
        ax.current_rot = create_quaternion(data)
        ax.draw_cube()
        plt.pause(period)
        
if __name__ == "__main__":
    main()