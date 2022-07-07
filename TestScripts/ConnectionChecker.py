import socket
import json
import pprint

HOST = '192.168.1.36'
PORT = 8893
BUFFER_SIZE = 1024


def exec_cmd(c):
    if 'command' not in c:
        msg = 'KeyError : Not found "command" key.'
        print(msg)
        return dict(status='Error', msg=msg)
    try:
        pprint.pprint(c)
        res = dict(is_sccess=True)
    except KeyError:
        cmd_arg = [s for s in c.keys() if s != 'command']
        msg = 'Invalid argument : "{0}" -> {{{1}}}'.format(
            c['command'], ", ".join(cmd_arg))
        print(msg)
        res = dict(is_sccess=False, msg=msg)

    return res


try:
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        # s.settimeout(3)
        s.bind((HOST, PORT))
        s.listen()
        while True:
            print("running...")
            c, client = s.accept()
            print('Client connected {0}'.format(client))
            try:
                cmd_dict = json.loads(c.recv(BUFFER_SIZE).decode('UTF-8'))
                res = exec_cmd(cmd_dict)
                print(res)
                c.send(json.dumps(res, ensure_ascii=False).encode())
            finally:
                c.close()
                if 'command' in cmd_dict:
                    if cmd_dict['command'] == 'Quit':
                        break
except    TimeoutError:
    print("timeout")
finally:
    s.close()
