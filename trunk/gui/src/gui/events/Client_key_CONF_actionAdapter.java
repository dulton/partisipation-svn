package gui.events;

import gui.Client;
import java.awt.event.ActionEvent;

/**
 * @author Anton Huttenlocher
 * @author Oleksiy Reznikov
 */


public class Client_key_CONF_actionAdapter implements
           java.awt.event.ActionListener {
       Client adaptee;

       public Client_key_CONF_actionAdapter(Client adaptee) {
           this.adaptee = adaptee;
       }

       public void actionPerformed(ActionEvent e) {
           adaptee.key_CONF_actionPerformed(e);
       }
   }
