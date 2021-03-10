use rodbus::prelude::*;

use std::net::SocketAddr;
use std::time::Duration;
use std::str::FromStr;


use tokio::time::delay_for;

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {

   let channel = spawn_tcp_client_task(
       SocketAddr::from_str("192.168.0.99:502")?,
       10,
       strategy::default()
   );

   let mut session = channel.create_session(
       UnitId::new(0x01),
       Duration::from_secs(1)
   );

   // try to poll for some coils every 3 seconds
   loop {
       match session.read_holding_registers(AddressRange::new(4064, 1)).await {
           Ok(values) => {
               for x in values {
                   println!("index: {} value: {}", x.index, x.value)
               }
           },
           Err(err) => println!("Error: {:?}", err)
       }

       delay_for(std::time::Duration::from_secs(3)).await
   }
}
