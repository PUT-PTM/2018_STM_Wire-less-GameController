using System.Collections;
using System.Collections.Generic;
using System.IO.Ports;
using UnityEngine;



public class PlayerController : MonoBehaviour {

    private SerialPort sp = new System.IO.Ports.SerialPort("COM6", 9600, System.IO.Ports.Parity.None, 8, System.IO.Ports.StopBits.One);
    private Transform player;
    public float speed = 0.2f;
    public float maxBound = 10.5f, minBound = -10.5f;
    public float rdata;
    

    public GameObject shot;
    public Transform shotSpawn;
    public float fireRate;
    private float nextFire;

    // Use this for initialization
    void Start () {
        sp.Open();
        player = GetComponent<Transform> ();
	}

    // Update is called once per frame
    void FixedUpdate()
    {
       
            var readData = sp.ReadChar();
            //char rdata = (char)readData;
            rdata = (float)readData;
            rdata -= 48;
            rdata /= 10;

        if (rdata != 5)
        {
            if (player.position.x < minBound && rdata < 0)

                rdata = 0;
            else if (player.position.x > maxBound && rdata > 0)

                rdata = 0;

            player.position += Vector3.right * rdata * speed;
        }
        if (rdata == 5 && Time.time > nextFire)
        {
            nextFire = Time.time + fireRate;
            Instantiate(shot, shotSpawn.position, shotSpawn.rotation);
        
        }
    }

   /* void Update()
    {
        if (rdata == 5 && Time.time > nextFire)
        {
            nextFire = Time.time + fireRate;
            Instantiate(shot, shotSpawn.position, shotSpawn.rotation);
        }
    }*/
}
