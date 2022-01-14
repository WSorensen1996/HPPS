################################################################################################
import socket
import os
from datetime import datetime
################################################################################################
"""
         _  _      _____ ______ _______      ________ _____  
     /\ | || |    / ____|  ____|  __ \ \    / /  ____|  __ \ 
    /  \| || |_  | (___ | |__  | |__) \ \  / /| |__  | |__) |
   / /\ \__   _|  \___ \|  __| |  _  / \ \/ / |  __| |  _  / 
  / ____ \ | |    ____) | |____| | \ \  \  /  | |____| | \ \ 
 /_/    \_\|_|___|_____/|______|_|  \_\  \/   |______|_|  \_\
"""
####    HELPER FUNCTIONS    ###########################################################################################

#   Checks if the given value is a date with the proper format 
def valid_date (date_string): 
    format = "%Y/%m/%d/%H:%M:%S"
    try:
        date = datetime.strptime(date_string, format)
        return date 
    except ValueError:
        return None 

#   Checks and returns: if found and path_file
def check_if_file(connection,file): 
    found = False
    if os.path.isfile(PATH + file ):
        path_file = PATH + file
    else: 
        path_file = PATH + "/" + file
    if not(os.path.isfile(path_file)): 
        print(f"[ERROR CODE 404]: FILE NOT FOUND!... {PATH} has no file:    {file}")
        listing = os.listdir(os.getcwd())
        print("Listing accessible files to client...", listing)
        response = bytearray(f"[ERROR CODE 404]: FILE NOT FOUND!... Accessible files in directory: {str(listing)}".encode())
        connection.sendall(response)
    if os.path.isfile(path_file):
        found=True
    return found , path_file

#   Checks and returns: file-type, file_type_accepted
def check_file_type(file): 
    #Checking if the client message file-type is an accepted file type
    #Outputs -1 if not found. 
    file_type_accepted = False
    file_type_is = None
    for i,x in enumerate([file.find(value,-(len(value))) for value in ACCEPTED_FILES]): 
        if x != -1:
            file_type_is = FILE_TYPES[0]+"/"+ACCEPTED_FILES[i]
            file_type_accepted = True
    return file_type_is, file_type_accepted


###     REQUEST HEADER HANDLERS #############################################################################################

#   Get method 
def get(connection, header, file) -> None  : 
    #get filetype: 
    file_type_is, file_type_accepted = check_file_type(file)
    #Check if file exists
    file_exists, path_file = check_if_file(connection, file)
    if file_type_accepted and file_exists : 

        if MODIFIED_SINCE_DATE != None: 
            if_Modified_Since(connection, header, file)
            return
        else:
            print("[200]: File will be returned to client...")
            with open(path_file, "r") as file:
                data = file.read()
                if data == " ": 
                    print("no data")
            content_type =  file_type_is
            content_length = len(data)
            now = datetime.now()
            date_time = now.strftime("%d/%m/%Y %H:%M:%S")
            response = bytearray(f"{ACCEPTED_PROTOCOL} 200 OK, \nDate: {date_time}, \nContent-Type: {content_type}; charset={CHARSET}, \nContent-Length: {content_length}, \nLocation: {path_file} \n\n\n {data}".encode())
            connection.sendall(response)
            return
        
    else : 
        print("[ERROR MESSAGE 304]: FILE NOT ACCEPTED")
        response = bytearray("[ERROR MESSAGE 304]: FILE NOT ACCEPTED".encode())
        connection.sendall(response)
    return

#   Accept
def accept(connection, header, format) -> None  : 
    all_possible_text_formats = [".doc",".docx",".odt",".pdf",".rtf",".tex",".wpd", ".txt",".csv"]
    no_format = [" "]
    cant_handle_format = True
    
    for i,x in enumerate([format.find(value,-(len(value))) for value in all_possible_text_formats]): 
        if x != -1:
            if any (xs == all_possible_text_formats[i] for xs in ACCEPTED_FILES):
                return
    
                
    
     
            ACCEPTED_FILES.append(all_possible_text_formats[i])
            print("[200]: FORMAT ACCEPTED")
            return
    for i,x in enumerate([format.find(value,-(len(value))) for value in no_format]): 
            if x != -1:
                print("Ugyl")
    if cant_handle_format: 
        print("[415]:  CANT HANDLE FORMAT OR FORMAT IS NOT TEXT")
        response = bytearray("[415]: CANT HANDLE FORMAT OR FORMAT IS NOT TEXT".encode())
        connection.sendall(response)
        cant_handle_format = False
    return

#   Get must run over modified if modified has been aktivated 
def if_Modified_Since(connection, header, file) -> None : 
    global MODIFIED_SINCE_DATE
    
    if MODIFIED_SINCE_DATE != None:  

        #Updates MODIFIED_SINCE_DATE if new date
        if valid_date(file) != None: 
            MODIFIED_SINCE_DATE = valid_date(file)
            return

        file_exists, path_file = check_if_file(connection, file)
        if file_exists :
            last_mod= os.path.getmtime(path_file)
            date_time_last_mod = datetime.fromtimestamp( last_mod )

            if MODIFIED_SINCE_DATE < date_time_last_mod: 
                file_type_is, file_type_accepted = check_file_type(file)

                print("[200]: File will be returned to client...")
                with open(path_file, "r") as file:
                    data = file.read()
                content_type =  file_type_is
                content_length = len(data)
                now = datetime.now()
                date_time = now.strftime("%d/%m/%Y %H:%M:%S")
                response = bytearray(f"{ACCEPTED_PROTOCOL} 200 OK, \nDate: {date_time}, \nContent-Type: {content_type}; charset={CHARSET}, \nContent-Length: {content_length}, \nLocation: {path_file} \n\n\n {data}".encode())
                connection.sendall(response)
                return

            if MODIFIED_SINCE_DATE > date_time_last_mod:
                print("[ERROR MESSAGE 304]: FILE WAS MODIFIED BEFORE DATE")
                response = bytearray("[ERROR MESSAGE 304]: FILE WAS MODIFIED BEFORE DATE".encode())
                connection.sendall(response)
                return

    else:
        #Check if valid date_string and updating MODIFIED_SINCE_DATE if so 
        MODIFIED_SINCE_DATE = valid_date(file) 
        if MODIFIED_SINCE_DATE != None: 
            print("[200]: DATE HAS BEEN UPDATED")
            return

        if MODIFIED_SINCE_DATE == None: 
            print("[ERROR MESSAGE 304]: MODIFIED_SINCE_DATE HAS NOT BEEN UPDATED")
            response = bytearray("[ERROR MESSAGE 304]: MODIFIED_SINCE_DATE HAS NOT BEEN UPDATED".encode())
            connection.sendall(response)
            return

    return

#   Host redirect the work directory
def host(connection, header, file) -> None: 
    try:
        if not(os.path.exists(file)): 
            raise AssertionError
        if os.path.isfile(file): 

            print("[ERROR CODE 400]:  A file is given... A path is needed)") 
            response = bytearray("[ERROR MESSAGE 400]:  A file is given... A path is needed".encode())
            connection.sendall(response)
            return
        if os.path.isdir(file): 
            global PATH 
            PATH = file
            print("[200]: DIRECTORY FOUND")
            return

    except AssertionError as a: 
        errormsg = "[ERROR CODE 404]: No path found at: "+str(file)
        print(errormsg)
        response = bytearray(errormsg.encode())
        connection.sendall(response)
    return

#   Keeps connection alive og closes it
def connection(connection, header, file) -> None:
    if file == "close": 
        global CONNECTION_WILL_CLOSE 
        CONNECTION_WILL_CLOSE = True
        return
    elif file == "keep-alive": 
        return
    else: 
        print("[ERROR CODE 400]:  BAD REQUEST)") 
        response = bytearray("[ERROR MESSAGE 400]:  BAD REQUEST".encode())
        connection.sendall(response)
        return 

#   Controlling request and mapping it to the proper method...
def msg_mapping(connection, msg): 

    #Splitting request into parts
    command_list = msg.split()
    continue_ = False

    try: 
        if (command_list[0] == "GET") and (command_list[2] == ACCEPTED_PROTOCOL): 
            continue_ = True 
    except IndexError as e: 
        print("[ERROR CODE 400] BAD REQUEST ")
        response = bytearray("[ERROR MESSAGE 400]:  BAD REQUEST".encode())
        connection.sendall(response)
        return 
    
    #Mapping the right commands to the proper functions
    if continue_: 
        for i,x in enumerate(ACCEPTED_REQUEST_HEADERS):
            for i__,x__ in enumerate(command_list): 
                if x == x__: 
                    ACCEPTED_REQUEST_HEADERS_FUNCS[i](connection, command_list[i__], command_list[i__+1])
        get(connection, command_list[0], command_list[1])
                   
    else: 
        print("[ERROR MESSAGE 406]: PROTOCOL NOT ACCEPTED")
        response = bytearray("[ERROR MESSAGE 406]: PROTOCOL NOT ACCEPTED".encode())
        connection.sendall(response)

#   SERVER STARING...
def receive_from_client():
    print("Starting server...")
    # Here we setup a socket for our clients to connect to
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
        server_socket.bind((SERVER_IP, SERVER_PORT))
        server_socket.listen()
        print("Server is listening... \n\n")
        while True:
            # Accept client requests. Note that each request is accepted on a
            # new socket identified by the 'connection' variable.
            connection, connection_address = server_socket.accept()
            with connection:
                print(f"\n\nServer messaged by {connection_address}")
                bytes_message = connection.recv(MSG_MAX_SIZE)
                if not bytes_message:
                    response = bytearray("No data received".encode())
                    connection.sendall(response)
                else:
                    string_message = bytes_message.decode(CHARSET)
                    print("MESSAGE FROM CLIENT: ", string_message)
                    msg_mapping(connection, string_message) 
                    if CONNECTION_WILL_CLOSE: 
                        break
    server_socket.close()

#   Recieving server adress...
def server_address():  
    # SERVER_IP input
    while 1:
        try:
            SERVER_IP = input("Enter your IP_adress:")
            socket.inet_aton(SERVER_IP)
            break

        except socket.error as e:
            print(e)
            continue

    # SERVER_PORT input
    while 1:   
        try: 
            SERVER_PORT = input("Enter your SERVER_PORT:")
            if SERVER_PORT == (int) :
                
                raise ValueError("[ERROR MESSAGE 405]: Enter digits only")
            SERVER_PORT = int(SERVER_PORT)
            break
        except ValueError as e: 
            print(e)
            continue

    # PATH input
    while 1: 
        try: 
            PATH = input("Enter your path:")
            if not(os.path.exists(PATH)): 
                raise AssertionError
            if os.path.isfile(PATH): 
                print("[ERROR CODE 400]: A file is given... A path is needed")
                continue
            if os.path.isdir(PATH): 
                print("[200]: Directory found! \n\n")
                break
        except AssertionError as a: 
            print("[ERROR CODE 404]: No path found at: "+str(PATH))
            continue

    return (SERVER_IP,SERVER_PORT, PATH) 


MSG_MAX_SIZE = 512
ACCEPTED_REQUEST_HEADERS = ["Accept:", "If-Modified-Since:", "Connection:",  "Host:"]
ACCEPTED_REQUEST_HEADERS_FUNCS = [accept, if_Modified_Since, connection, host]
ACCEPTED_FILES = [".html"]
FILE_TYPES = ["text"]
CHARSET = "utf-8"
ACCEPTED_PROTOCOL = "HTTP/1.1"
CONNECTION_WILL_CLOSE = False
MODIFIED_SINCE_DATE = None 



if __name__ == "__main__":
    SERVER_IP, SERVER_PORT, PATH = server_address() 
    receive_from_client()
    