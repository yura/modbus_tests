pub fn read_value() -> Result<u16, Box<dyn std::error::Error>> {
    use tokio_modbus::prelude::*;

    let socket_addr = "192.168.0.99:502".parse()?;
    let mut client = client::sync::tcp::connect(socket_addr)?;
    client.set_slave(Slave::from(1));
    let data = client.read_holding_registers(4064, 1)?;
    println!("Response is '{:?}'", data);

    Ok(data[0])
}

fn main() {
    let value = read_value();
    println!("Response is '{:?}'", value);
}
