set pagination off
set confirm off
set remotetimeout 90
target extended-remote localhost:3333
monitor halt
delete

printf "\n=== EXPERIMENT: replicate RunOnlineMode's boot sequence by hand ===\n"
printf "State before: G_u8TcpConnected=%d G_u8WifiConnected=%d\n", G_u8TcpConnected, G_u8WifiConnected

set $conn = (int)HESP8266_u8EnsureConnection("Ahmedlaptop", "katkot123", "185.176.43.100", "80", 3)
printf "\nHESP8266_u8EnsureConnection() -> %d   (0=OK 1=TIMEOUT 2=ERROR 3=NOT_CONNECTED)\n", $conn
printf "G_u8TcpConnected = %d\n", G_u8TcpConnected
printf "G_u8WifiConnected = %d\n", G_u8WifiConnected
printf "G_u8HESP8266LastStatus = %d\n", G_u8HESP8266LastStatus

if $conn == 0
  printf "\n--- connection is up: now read /status/ota.txt the way OTA_vCheckForUpdate does ---\n"
  set $rc = (int)HESP8266_u8HttpGet("/status/ota.txt", G_pcUpdateUrl, 64)
  printf "\nHESP8266_u8HttpGet(\"/status/ota.txt\") -> %d   (0=OK 1=TIMEOUT 2=ERROR 3=NOT_CONNECTED)\n", $rc
  printf "last response line seen = %s\n", G_cHESP8266LastResponse
  printf "buffer returned        = %s\n", G_pcUpdateUrl
end

detach
quit