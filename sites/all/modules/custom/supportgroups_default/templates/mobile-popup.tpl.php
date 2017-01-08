<div id='mobile-popup'>
	<div id='mobile-popup-inner'>
		<div id='close-button' class='close-button'></div>
		<h2>Login or Register</h2>
		<p>You are visiting <b>Support Groups</b> as an anonymous user.</p>
		<p>Please consider joining our community and gain access to additional features by</p>
		<p><a onclick='return false;' id='mobile-register'>registering</a> or <a onclick='return false;' id='mobile-login'>logging into your account</a></p>
	</div>
</div>
<style>

	#mobile-popup {
		top: 120px;
		position: fixed;
		z-index:2000;
		display: none;
		width: 100%;
	}
	#mobile-popup-inner {
		max-width:550px;
		margin:0 auto;
		color:#666666;
		background-color: white;
		position: relative;
		padding: 2em; }
		#mobile-popup div.close-button {
			position: absolute;
			right: 5px;
			top: 5px;
			width: 20px;
			height: 20px;
			background-image: url(/sites/all/modules/custom/supportgroups_default/images/close.png);
			background-position: center center;
			background-repeat: no-repeat; }
			#user-register-form div.close-button:hover{
				cursor: pointer; }

	#mobile-popup h2 {
		font-size:1.7em;
		margin-top:0; }
	#mobile-popup-inner a {
		text-decoration: underline;
		font-weight: bold;}
</style>