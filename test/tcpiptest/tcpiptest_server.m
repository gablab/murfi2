% simple example tcpip server (simple == crappy)
%
% Todd Thompson <toddt@mit.edu>
% Oliver Hinds <ohinds@mit.edu> 2008-04-14
function tcpiptest_server(output_port, output_host, timeout)
  if(nargin < 1)
    output_port = 15001;
  end

  if(nargin < 2)
    output_host = 'localhost';
  end

  if(nargin < 3)
    timeout = 20*10000;
  end

  % required java libs
  import java.net.ServerSocket
  import java.net.InetSocketAddress
  import java.io.*

  % start listening forever
  while(1)
    % set up the server socket
    address = InetSocketAddress(output_host, output_port);
    server_socket = ServerSocket;
    server_socket.bind(address);
    server_socket.setSoTimeout(timeout);
    server_socket.setReuseAddress(true);

    % setup the streams
    skt = server_socket.accept;
    output_stream = skt.getOutputStream;
    buffered_output_stream = BufferedOutputStream(output_stream);
    d_output_stream = DataOutputStream(buffered_output_stream);
    input_stream = skt.getInputStream;
    buffered_input_stream = BufferedInputStream(input_stream);
    d_input_stream = DataInputStream(buffered_input_stream);

    % receive a message
    msg = [];
    while d_input_stream.available
      s = d_input_stream.read();
      msg = [msg char(s)];
      %d_output_stream.writeChars(msg);
    end
    if ~(isempty(msg)) disp(msg); end

    % cleanup
    skt.close
    output_stream.close
    buffered_output_stream.close
    d_output_stream.close
    input_stream.close
    buffered_input_stream.close
    d_input_stream.close
    server_socket.close
  end


return