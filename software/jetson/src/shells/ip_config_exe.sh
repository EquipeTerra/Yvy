#!/bin/bash

# Senha do sudo
PASSWORD="equipeterra"

# Cria o script de configuração do IP fixo
echo "$PASSWORD" | sudo -S bash -c 'cat > /usr/local/bin/ip_configlib.sh' << 'EOF'
#!/bin/bash

IFACE="eth0"
IP_ADDR="192.168.0.3"
NETMASK="24"
GATEWAY="192.168.0.1"

# Ativa a interface
sudo ip link set $IFACE up

# Remove IPs antigos
sudo ip addr flush dev $IFACE

# Adiciona IP fixo
sudo ip addr add ${IP_ADDR}/${NETMASK} dev $IFACE

# Define o gateway
sudo ip route add default via $GATEWAY dev $IFACE
EOF

# Dá permissão de execução
echo "$PASSWORD" | sudo -S chmod +x /usr/local/bin/ip_configlib.sh

# Cria o service do systemd
echo "$PASSWORD" | sudo -S bash -c 'cat > /etc/systemd/system/fix-eth0.service' << 'EOF'
[Unit]
Description=Set static IP for eth0
After=network-pre.target
Wants=network-pre.target

[Service]
Type=oneshot
ExecStart=/usr/local/bin/ip_configlib.sh
RemainAfterExit=true

[Install]
WantedBy=multi-user.target
EOF

# Recarrega e ativa o serviço
echo "$PASSWORD" | sudo -S systemctl daemon-reload
echo "$PASSWORD" | sudo -S systemctl enable fix-eth0.service

echo "✅ IP fixo 192.168.0.3 configurado para eth0. Será aplicado automaticamente no próximo boot!"
