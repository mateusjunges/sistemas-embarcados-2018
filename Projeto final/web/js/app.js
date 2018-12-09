var vm = new Vue({
    el: '#app',
    data: {
        connection: {
            hostname: 'broker.mqtt-dashboard.com',
            port: 8000,
            clientId: 'mqttjs_' + Math.random().toString(16).substr(2, 8),
            username: '',
            password: ''
        },
        subscriptions: [],
        subscription: {
            topic: 'mateusjunges.alceubritto.projeto.sistemas.embarcados.2018',
            qos: 2,
            color: '#ff0000'
        },
        publication: {
            topic: 'mateusjunges.alceubritto.projeto.sistemas.embarcados.2018',
            qos: 2,
            retain: false,
            message: ''
        },
        messages: [],
        client: {
            connected: false
        },
        horario: ''
    },
    methods: {
        connect: function() {
            var domain = 'ws://' + this.connection.hostname + ':' + this.connection.port + '/mqtt';
            this.client = mqtt.connect(domain, {
                reconnectPeriod: 10000
            });
            this.client.on('connect', function() {
                $('#connectionModal').modal('hide');
                swal({
                   icon: 'success',
                   title: 'Sucesso!',
                   text: 'MQTT conectado com sucesso!',
                   timer: 3000,
                });
            });
            this.client.on('message', function(topic, message, packet) {
                var payload = message.toString();
                vm.messages.push({
                    color: vm.getColorForTopic(topic),
                    isJSON: vm.isJSON(payload),
                    datetime: new Date().toLocaleString(),
                    topic: topic,
                    qos: packet.qos,
                    retain: false,
                    payload: payload
                });
            });
            this.client.on('reconnect', function() {
            });
            this.client.on('close', function() {
            });
            this.client.on('offline', function() {
                swal({
                   icon: 'error',
                   title: 'Erro!',
                   text: 'MQTT desconectado',
                });
            });
            this.client.on('error', function(error) {
            });
            this.client.on('end', function() {
            });
        },
        disconnect: function() {
            this.client.end();
            this.subscriptions = [];
            this.messages = [];
        },
        publish: function() {
            this.client.publish(
                this.publication.topic,
                this.publication.message,
                {
                    qos: this.publication.qos,
                    retain: this.publication.retain
                },
                function(err) {
                    if (err) {
                        alert(err);
                    } else {
                        vm.publication.message = 'L';
                    }
                }
            );

        },
        publish2: function() {
            this.client.publish(
                this.publication.topic, 
                this.publication.message, 
                {
                    qos: this.publication.qos, 
                    retain: this.publication.retain
                }, 
                function(err) {
                    if (err) {
                        alert(err);
                    } else {
                        vm.publication.message = 'D';
                    }
                }
            );

        },
        subscribe: function(event) {
            this.client.subscribe(
                this.subscription.topic, 
                {
                    qos: this.subscription.qos
                },
                function(err, granted) {
                    if (err) {
                        alert(err);
                    } else {
                        for (var i = 0; i < granted.length; i++) {
                            vm.subscriptions.push({
                                topic: granted[i].topic, 
                                qos: granted[i].qos,
                                color: vm.subscription.color
                            });
                        }
                        vm.subscription.topic = 'mateusjunges.alceubritto.projeto.sistemas.embarcados.2018';
                    }
                    if(!err)
                        swal({
                            icon: 'success',
                            title: 'Sucesso!',
                            text: 'Topico conectado'
                        });
            });
        },
        unsubscribe: function(subscription) {
            this.client.unsubscribe(subscription.topic, function(err) {
                if (err) {
                    alert(err);
                }
            });
            var index = this.subscriptions.indexOf(subscription);
            if (index > -1) {
                this.subscriptions.splice(index, 1);
            }
            // Also remove all messages that were generated as a result of that subscription.
            var i = this.messages.length;
            while (i--) {
                if (mqttWildcard(this.messages[i].topic, subscription.topic) != null) {
                    this.messages.splice(i, 1);
                }
            }
        },
        // toggleRetain: function() {
        //     this.publication.retain = !this.publication.retain;
        // },
        isJSON: function(str) {
            try {
                return (JSON.parse(str) && !!str);
            } catch (e) {
                return false;
            }
        },
        getColorForTopic: function(topic) {
            for (var i = 0; i < this.subscriptions.length; i++) {
                if (mqttWildcard(topic, this.subscriptions[i].topic) != null) {
                    console.log(this.subscriptions[i].color);
                    return this.subscriptions[i].color;
                }
            }
            return null;
        }
    }
});
